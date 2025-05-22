using Microsoft.AspNetCore.Mvc;
using System.Text;

var builder = WebApplication.CreateBuilder(args);

// Configure JSON serialization to match Go's output
builder.Services.AddControllers()
    .AddJsonOptions(options =>
    {
        options.JsonSerializerOptions.PropertyNamingPolicy = null; // Use property names as-is (no camelCase)
    });

var app = builder.Build();

// Enable routing for controllers
app.MapControllers();

// Run the server
app.Run();

// Controller for handling endpoints
[ApiController]
public class RandomStringController : ControllerBase
{
    private const string Population = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    private static readonly Random _random = new Random();

    // Helper method to generate random string
    private static string GenerateRandomString(int length)
    {
        var builder = new StringBuilder(length);
        for (int i = 0; i < length; i++)
        {
            builder.Append(Population[_random.Next(Population.Length)]);
        }
        return builder.ToString();
    }

    // Root endpoint: GET /
    [HttpGet("/")]
    public IActionResult Index()
    {
        return Ok(new { message = GenerateRandomString(100) });
    }

    // Items endpoint: GET /items/{item_id}
    [HttpGet("/items/{item_id}")]
    public IActionResult ReadItem(int item_id)
    {
        return Ok(new { item_id, message = GenerateRandomString(100) });
    }
}