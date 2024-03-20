package main

import (
	"context"
	"log"
	"net/http"
	"server/app"
	"server/models"
	"server/utils"

	"github.com/gorilla/websocket"
	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

var clients = make(map[*websocket.Conn]bool) // Connected clients
var clients_ip = make(map[string]string)     // Connected clients - ip -> username
var broadcast = make(chan models.Message)    // Broadcast channel

var activeChat = models.Chat{
	Name:     "global",
	ChatData: models.Data{},
	IsGroup:  false,
}

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

func main() {
	// Load environmental variables
	requiredEnvVars := []string{"MONGODB_URL", "MONGODB_DB"}
	utils.LoadEnvVarialbes(requiredEnvVars)
	// Init mongodb connection
	app.InitMongoDB()

	// Load global chat - default behavior
	utils.LoadChat("global", &activeChat)

	// Configure websocket route
	http.HandleFunc("/ws", handleConnections)

	// Start listening for incoming chat messages
	go handleMessages()

	// Start the server on localhost port 8000 and log any errors
	log.Println("Server started on :8000")
	err := http.ListenAndServe(":8000", nil)
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
	initialData := models.Message{
		Type:      "data",
		Sender:    "server",
		Recipient: ip,
		Content:   activeChat.ChatData,
	}

	client := app.GetMongoClient()
	chat_collection := client.Database(app.MongoDB).Collection("chats")

	for {
		var msg models.Message

		// Read in a new message as JSON and map it to a Message object
		err := ws.ReadJSON(&msg)
		if err != nil {
			log.Printf("Message read: %v", err)
			delete(clients, ws)
			break
		}

		// Hanlde chat switch
		if msg.Type == "switch" {
			content, ok := msg.Content.(string)
			if !ok {
				log.Print("Err - switching the chat")
			} else {
				utils.LoadChat(content, &activeChat)
			}
		}

		// ------------------

		user_found := utils.UserFind(msg.Sender, activeChat.ChatData.Users)

		if user_found == nil {
			var usr models.User
			usr.Username = msg.Sender
			usr.Password = "TEST"
			if msg.Type != "switch" {
				usr.Messages = append(usr.Messages, msg)
			}
			clients_ip[ip] = msg.Sender
			activeChat.ChatData.Users = append(activeChat.ChatData.Users, usr)
			initialData.Content = activeChat.ChatData

			filter := bson.M{"name": activeChat.Name}
			update := bson.M{"$set": bson.M{"chatData": activeChat.ChatData}}
			mongoErr := chat_collection.FindOneAndUpdate(context.Background(), filter, update)
			if mongoErr != nil && mongoErr.Err() != nil {
				if mongoErr.Err() == mongo.ErrNoDocuments {
					var usr models.User
					usr.Username = "global"
					activeChat.ChatData.Users = append(activeChat.ChatData.Users, usr)
					_, mongoErr2 := chat_collection.InsertOne(context.Background(), activeChat)
					if mongoErr2 != nil {
						log.Printf("Mongo err - insert user message: %v", mongoErr2)
					}
				} else {
					log.Printf("Mongo err - new user push to active chat: %v", mongoErr.Err())
				}
			}
			user_found = &usr
		}
		if msg.Type != "switch" {
			activeChat.ChatData.Messages = append(activeChat.ChatData.Messages, msg)
			user_found.Messages = append(user_found.Messages, msg)
		}

		filter := bson.M{"name": activeChat.Name}
		update := bson.M{"$set": bson.M{"chatData": activeChat.ChatData}}
		mongoErr := chat_collection.FindOneAndUpdate(context.Background(), filter, update)
		if mongoErr != nil && mongoErr.Err() != nil {
			if mongoErr.Err() == mongo.ErrNoDocuments {
				_, mongoErr2 := chat_collection.InsertOne(context.Background(), activeChat)
				if mongoErr2 != nil {
					log.Printf("Mongo err - insert user message: %v", mongoErr2)
				}
			} else {
				log.Printf("Mongo err - update user messages: %v", mongoErr.Err())
			}
		}

		initialData.Content = activeChat.ChatData

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
				log.Printf("Message send: %v", err)
				client.Close()
				delete(clients, client)
			}
		}
	}
}
