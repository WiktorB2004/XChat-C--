package utils

import (
	"context"
	"log"
	"server/app"
	"server/models"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

func LoadChat(name string, activeChat *models.Chat) {
	client := app.GetMongoClient()
	chat_collection := client.Database(app.MongoDB).Collection("chats")

	filter := bson.M{"name": name}
	// Load global chat
	mongoErr := chat_collection.FindOne(context.Background(), filter).Decode(&activeChat)
	if mongoErr != mongo.ErrNoDocuments && mongoErr != nil {
		log.Fatal("Mongo err - loading chat: ", mongoErr)
	}
	log.Printf("Reloaded the chat to: %v", name)
}
