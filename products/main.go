package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

type Product struct {
	ID    interface{} `bson:"_id,omitempty" json:"id"`
	Name  string      `bson:"name" json:"name"`
	Price float64     `bson:"price" json:"price"`
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
	mongoUri := "mongodb://root:example@" + mongoHost + ":" + mongoPort
	mongoDb := getEnvOr("MONGO_DB", "app")
	mongoCollection := getEnvOr("MONGO_COLLECTION", "products")
	port := getEnvOr("PORT", "8082")

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

	http.HandleFunc("/api/v1/products", func(w http.ResponseWriter, r *http.Request) {
		// Handle preflight
		if r.Method == http.MethodOptions {
			w.WriteHeader(http.StatusNoContent)
			return
		}

		switch r.Method {
		case http.MethodGet:
			ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
			defer cancel()

			cursor, err := coll.Find(ctx, bson.M{})
			if err != nil {
				log.Printf("❌ Error during Find(): %v\n", err)
				http.Error(w, "Database error", http.StatusInternalServerError)
				return
			}

			defer cursor.Close(ctx)

			var products []Product
			if err := cursor.All(ctx, &products); err != nil {
				http.Error(w, "Error reading data", http.StatusInternalServerError)
				return
			}

			w.Header().Set("Content-Type", "application/json")
			json.NewEncoder(w).Encode(map[string]any{"products": products})
			return

		case http.MethodPost:
			var p Product

			// Decode body
			if err := json.NewDecoder(r.Body).Decode(&p); err != nil {
				http.Error(w, "Invalid JSON", http.StatusBadRequest)
				return
			}
			log.Println("Received body:", p)
			if p.Name == "" || p.Price <= 0 {
				http.Error(w, "Missing or invalid fields", http.StatusBadRequest)
				return
			}

			ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
			defer cancel()

			// Insert into Mongo
			result, err := coll.InsertOne(ctx, p)
			if err != nil {
				log.Printf("❌ Insert error: %v\n", err)
				http.Error(w, "Database error", http.StatusInternalServerError)
				return
			}

			p.ID = result.InsertedID // return the saved object with the ID assigned

			w.Header().Set("Content-Type", "application/json")
			w.WriteHeader(http.StatusCreated)
			json.NewEncoder(w).Encode(p)
			return

		default:
			log.Print("etamo po aqui")
			http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
			return
		}
	})
	http.ListenAndServe(":"+port, nil)
}
