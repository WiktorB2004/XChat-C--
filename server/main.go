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

var clients = make(map[*websocket.Conn]bool)          // Connected clients
var clientUsername = make(map[string]*websocket.Conn) // Connected clients - ws -> username
var activeUsersChats = make(map[string]string)        // Current user chat - username -> chat
var broadcast = make(chan models.Message)             // Broadcast channel

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
			chatName, ok := msg.Content.(string)
			if !ok {
				log.Print("Err - switching the chat")
			} else {
				// Check if this is a direct chat
				_, ok = clientUsername[msg.Sender]
				if ok {
					if msg.Sender < chatName {
						activeUsersChats[msg.Sender] = msg.Sender + "/" + chatName
					} else {
						activeUsersChats[msg.Sender] = chatName + "/" + msg.Sender
					}
					activeChat = models.Chat{}
					activeChat.Name = activeUsersChats[msg.Sender]
					activeChat.ChatData = models.Data{}
					var usr models.User
					usr.Username = "global"
					activeChat.ChatData.Users = append(activeChat.ChatData.Users, usr)
					activeChat.IsGroup = false
				} else {
					activeUsersChats[msg.Sender] = chatName
				}
			}
		}
		// ------------------

		userChat, exists := activeUsersChats[msg.Sender]
		if exists {
			err = utils.LoadChat(userChat, &activeChat)
			if err != nil {
				if err == mongo.ErrNoDocuments {
					_, mongoErr := chat_collection.InsertOne(context.Background(), activeChat)
					if mongoErr != nil {
						log.Printf("Mongo err - insert user message: %v", mongoErr)
					}
				} else {
					log.Printf("Err - loading the chat: %v", err)
				}
			}
		} else {
			activeUsersChats[msg.Sender] = "global"
			clientUsername[msg.Sender] = ws
		}

		user_found := utils.UserFind(msg.Sender, activeChat.ChatData.Users)

		if user_found == nil {
			var usr models.User
			usr.Username = msg.Sender
			usr.Password = "TEST"
			if msg.Type != "switch" {
				usr.Messages = append(usr.Messages, msg)
			}
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

		// broadcast <- initialData
		sendChatMessage(activeUsersChats[msg.Sender], initialData)

		broadcast <- msg
	}
}

// FIXME: Check and correct clients / clientsUseranme deletion
func handleMessages() {
	for {
		// Grab the next message from the broadcast channel
		msg := <-broadcast
		// Send it out to every client that is currently connected
		for client := range clients {
			err := client.WriteJSON(msg)
			if err != nil {
				log.Printf("Error - message send: %v", err)
				client.Close()
				delete(clients, client)
			}
		}
	}
}

func sendChatMessage(chat string, msg models.Message) {
	for username, conn := range clientUsername {
		if activeUsersChats[username] == chat {
			err := conn.WriteJSON(msg)
			if err != nil {
				log.Printf("Client specific message sending failed: %v", msg)
			}
		}
	}
}
