package utils

import (
	"log"
	"os"

	"github.com/joho/godotenv"
)

func LoadEnvVarialbes(required []string) {
	// LOAD .env variables
	if err := godotenv.Load(".env"); err != nil {
		log.Println("Warning: .env file not found or unable to load.")
	}

	// Check if required environment variables are set

	for _, envVar := range required {
		if _, exists := os.LookupEnv(envVar); !exists {
			log.Fatalf("Error: Required environment variable %s is not set.", envVar)
		}
	}
}
