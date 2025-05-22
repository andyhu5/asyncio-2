package main

import (
	"math/rand"
	"strings"
	"time"

	"github.com/gofiber/fiber/v2"
)

// Precompute the population for random string generation
const population = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

func main() {
	// Initialize Fiber app
	app := fiber.New()

	// Seed random number generator
	rand.Seed(time.Now().UnixNano())

	// Root endpoint
	app.Get("/", func(c *fiber.Ctx) error {
		return c.JSON(fiber.Map{
			"message": generateRandomString(100),
		})
	})

	// Items endpoint with ID parameter
	app.Get("/items/:item_id", func(c *fiber.Ctx) error {
		itemID, err := c.ParamsInt("item_id")
		if err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"error": "Invalid item_id",
			})
		}
		return c.JSON(fiber.Map{
			"item_id": itemID,
			"message": generateRandomString(100),
		})
	})

	// Start server
	app.Listen(":8080")
}

// Helper function to generate random string
func generateRandomString(length int) string {
	var builder strings.Builder
	builder.Grow(length)
	for i := 0; i < length; i++ {
		builder.WriteByte(population[rand.Intn(len(population))])
	}
	return builder.String()
}
