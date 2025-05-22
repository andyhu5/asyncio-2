use actix_web::{web, App, HttpResponse, HttpServer, Responder};
use rand::{rngs::ThreadRng, Rng};
use std::string::String;

// Precompute the population for random string generation
const POPULATION: &str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// Helper function to generate random string
fn generate_random_string(length: usize, rng: &mut ThreadRng) -> String {
    let mut result = String::with_capacity(length);
    for _ in 0..length {
        let idx = rng.gen_range(0..POPULATION.len());
        result.push(POPULATION.as_bytes()[idx] as char);
    }
    result
}

// Root endpoint
async fn index() -> impl Responder {
    let mut rng = rand::thread_rng();
    let message = generate_random_string(100, &mut rng);
    HttpResponse::Ok().json(serde_json::json!({
        "message": message
    }))
}

// Items endpoint with ID parameter
async fn read_item(path: web::Path<i32>) -> impl Responder {
    let item_id = path.into_inner();
    let mut rng = rand::thread_rng();
    let message = generate_random_string(100, &mut rng);
    HttpResponse::Ok().json(serde_json::json!({
        "item_id": item_id,
        "message": message
    }))
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        App::new()
            .route("/", web::get().to(index))
            .route("/items/{item_id}", web::get().to(read_item))
    })
    .bind("0.0.0.0:8080")?
    .run()
    .await
}