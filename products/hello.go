package main

import (
	"encoding/json"
	"net/http"
)

type Product struct {
	ID    int     `json:"id"`
	Name  string  `json:"name"`
	Price float64 `json:"price"`
}

func main() {
	http.HandleFunc("/api/v1/products", read)
	http.ListenAndServe(":8084", nil)
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
