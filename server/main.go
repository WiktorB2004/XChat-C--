package main

import (
	"context"
	"log"
	"net/http"
	"os"
	"server/app"

	"github.com/gorilla/websocket"
	"github.com/joho/godotenv"
	"go.mongodb.org/mongo-driver/bson"
)

var clients = make(map[*websocket.Conn]bool) // Connected clients
var clients_ip = make(map[string]string)     // Connected clients - ip -> username
var broadcast = make(chan Message)           // Broadcast channel

// Define our message object
type Message struct {
	ID        string      `json:"id,omitempty" bson:"_id,omitempty"`
	Sender    string      `json:"sender,omitempty" bson:"sender,omitempty"`
	Type      string      `json:"type" bson:"type"`
	Recipient string      `json:"recipient,omitempty" bson:"recipient,omitempty"`
	Content   interface{} `json:"content,omitempty" bson:"content,omitempty"`
}

type Data struct {
	Users    []string  `json:"users"`
	Messages []Message `json:"messages"`
}

type User struct {
	Username string    `json:"username" bson:"username"`
	Password string    `json:"password" bson:"password"`
	Messages []Message `json:"Messages" bson:"Messages"`
}

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

// TO MOVE OUT
func contains(s []string, str string) bool {
	for _, v := range s {
		if v == str {
			return true
		}
	}
	return false
}

// TO MOVE OUT

var userData = Data{
	Users:    []string{},
	Messages: []Message{},
}

func main() {
	// LOAD .env variables
	if err := godotenv.Load(".env"); err != nil {
		log.Println("Warning: .env file not found or unable to load.")
	}

	// Check if required environment variables are set
	requiredEnvVars := []string{"MONGODB_URL", "MONGODB_DB"}
	for _, envVar := range requiredEnvVars {
		if _, exists := os.LookupEnv(envVar); !exists {
			log.Fatalf("Error: Required environment variable %s is not set.", envVar)
		}
	}
	//

	// Init mongodb connection
	app.InitMongoDB()
	client := app.GetMongoClient()
	message_collection := client.Database(app.MongoDB).Collection("messages")

	// FIXME: Correct error handling
	// Define an empty filter to match all documents
	filter := bson.M{}

	// Find all documents in the collection
	cursor, err := message_collection.Find(context.Background(), filter)
	if err != nil {
		log.Fatal("Err: ", err)
	}

	// Iterate over the cursor and decode each document into a User struct
	for cursor.Next(context.Background()) {
		var msg Message
		cursor.Decode(&msg)

		// Append the decoded document to the slice
		userData.Messages = append(userData.Messages, msg)
	}

	// Check for any cursor errors after the loop
	if err = cursor.Err(); err != nil {
		log.Fatal("Err: ", err)
	}

	// Close the cursor once done
	cursor.Close(context.Background())

	// MONGODB Connection - Handle the errors properly

	// Configure websocket route
	http.HandleFunc("/ws", handleConnections)

	// Start listening for incoming chat messages
	go handleMessages()

	// Start the server on localhost port 8000 and log any errors
	log.Println("Server started on :8000")
	err = http.ListenAndServe(":8000", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}

func handleConnections(w http.ResponseWriter, r *http.Request) {
	// Upgrade initial GET request to a websocket
	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Fatal(err)
	}
	// Make sure we close the connection when the function returns
	defer ws.Close()

	// Register our new client
	clients[ws] = true

	// Get the IP address of the connected user
	ip := r.RemoteAddr
	// Log the new connection
	log.Println("New connection established:", ip)

	// Send initial data to the client
	initialData := Message{
		Type:      "data",
		Sender:    "server",
		Recipient: ip,
		Content:   userData,
	}

	// Get the MongoDB client
	client := app.GetMongoClient()

	// Access the "messages" collection in the specified database
	message_collection := client.Database(app.MongoDB).Collection("messages")
	user_collection := client.Database(app.MongoDB).Collection("users")
	//

	for {
		var msg Message
		var usr User
		// Read in a new message as JSON and map it to a Message object
		err := ws.ReadJSON(&msg)
		if err != nil {
			log.Printf("error: %v", err)
			delete(clients, ws)
			break
		}

		filter := bson.M{"username": msg.Sender}
		user_found := user_collection.FindOne(context.Background(), filter).Decode(&usr)
		if user_found != nil {
			usr.Username = msg.Sender
			usr.Password = "TEST"
		}
		usr.Messages = append(usr.Messages, msg)
		if !contains(userData.Users, msg.Sender) {
			clients_ip[ip] = msg.Sender
			userData.Users = append(userData.Users, msg.Sender)
			initialData.Content = userData
			if user_found != nil {
				_, err = user_collection.InsertOne(context.Background(), usr)
			} else {
				update := bson.M{"$set": bson.M{"Messages": usr.Messages}}
				_, err = user_collection.UpdateOne(context.Background(), filter, update)
			}
			if err != nil {
				log.Printf("error: %v", err)
				break
			}
		}

		// Insert the test message into the collection
		_, err = message_collection.InsertOne(context.Background(), msg)
		if err != nil {
			log.Printf("error: %v", err)
			break
		}
		update := bson.M{"$set": bson.M{"Messages": usr.Messages}}
		_, err = user_collection.UpdateOne(context.Background(), filter, update)
		// Check for errors during insertion
		if err != nil {
			log.Printf("error: %v", err)
			break
		}

		userData.Messages = append(userData.Messages, msg)
		initialData.Content = userData

		broadcast <- initialData
		broadcast <- msg
	}
}

func handleMessages() {
	for {
		// Grab the next message from the broadcast channel
		msg := <-broadcast
		// Send it out to every client that is currently connected
		for client := range clients {
			err := client.WriteJSON(msg)
			if err != nil {
				log.Printf("error: %v", err)
				client.Close()
				delete(clients, client)
			}
		}
	}
}
