package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

type Product struct {
	ID    int     `json:"id"`
	Name  string  `json:"name"`
	Price float64 `json:"price"`
}

func getEnvOr(key, defaultValue string) string {
	if value := os.Getenv(key); value != "" {
		return value
	}
	return defaultValue
}

func main() {
	mongoHost := getEnvOr("MONGO_HOST", "localhost")
	mongoPort := getEnvOr("MONGO_PORT", "27017")
	mongoUri := "mongodb://" + mongoHost + ":" + mongoPort
	mongoDb := getEnvOr("MONGO_DB", "app")
	mongoCollection := getEnvOr("MONGO_COLLECTION", "products")
	port := getEnvOr("PORT", "8083")

	// create context with timeout
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()

	// connect to Mongo
	clientOptions := options.Client().ApplyURI(mongoUri)
	client, err := mongo.Connect(ctx, clientOptions)
	if err != nil {
		log.Fatalf("❌ Error connecting to MongoDB: %v", err)
	}

	// ping the database
	if err := client.Ping(ctx, nil); err != nil {
		log.Fatalf("❌ Cannot reach MongoDB: %v", err)
	}

	fmt.Println("✅ Connected to MongoDB!")

	// example: get a collection
	db := client.Database(mongoDb)
	coll := db.Collection(mongoCollection)
	fmt.Println("📦 Ready to use collection:", coll.Name())

	http.HandleFunc("/api/v1/products", read)
	http.ListenAndServe(":"+port, nil)
}

func read(w http.ResponseWriter, r *http.Request) {
	// Hardcoded products
	products := []Product{
		{ID: 1, Name: "Laptop", Price: 999.99},
		{ID: 2, Name: "Mouse", Price: 25.50},
		{ID: 3, Name: "Keyboard", Price: 49.90},
	}

	// Set header so client knows it’s JSON
	w.Header().Set("Content-Type", "application/json")

	// Encode to JSON and write to response
	if err := json.NewEncoder(w).Encode(products); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}
