package utils

import (
	"context"
	"log"
	"server/app"
	"server/models"

	"go.mongodb.org/mongo-driver/bson"
)

func LoadChat(name string, activeChat *models.Chat) error {
	client := app.GetMongoClient()
	chat_collection := client.Database(app.MongoDB).Collection("chats")

	filter := bson.M{"name": name}
	// Load global chat
	mongoErr := chat_collection.FindOne(context.Background(), filter).Decode(&activeChat)
	if mongoErr != nil {
		return mongoErr
	}
	log.Printf("Reloaded the chat to: %v", name)
	return nil
}
