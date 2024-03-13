package main

import (
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

var clients = make(map[*websocket.Conn]bool) // Connected clients
var clients_ip = make(map[string]string)     // Connected clients - ip -> username
var broadcast = make(chan Message)           // Broadcast channel

// Define our message object
type Message struct {
	Sender    string      `json:"sender,omitempty"`
	Type      string      `json:"type"`
	Recipient string      `json:"recipient,omitempty"`
	Content   interface{} `json:"content,omitempty"`
}

type Data struct {
	Users    []string  `json:"users"`
	Messages []Message `json:"messages"`
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

func main() {
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

var userData = Data{
	Users:    []string{},
	Messages: []Message{},
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

	for {
		var msg Message
		// Read in a new message as JSON and map it to a Message object
		err := ws.ReadJSON(&msg)
		if err != nil {
			log.Printf("error: %v", err)
			delete(clients, ws)
			break
		}

		if !contains(userData.Users, msg.Sender) {
			clients_ip[ip] = msg.Sender
			userData.Users = append(userData.Users, msg.Sender)
			initialData.Content = userData
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
