#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <random>
#include <sstream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// Generate a random alphanumeric string of length k
std::string random_string(std::size_t k) {
    static const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static thread_local std::mt19937 rg{std::random_device{}()};
    static thread_local std::uniform_int_distribution<std::string::size_type> pick(0, charset.size() - 1);
    std::string s;
    s.reserve(k);
    for (std::size_t i = 0; i < k; ++i) {
        s += charset[pick(rg)];
    }
    return s;
}

// Return a stringified JSON for /
std::string get_index_json() {
    return "{\"message\":\"" + random_string(100) + "\"}";
}

// Return a stringified JSON for /items/{item_id}
std::string get_item_json(int item_id) {
    std::ostringstream oss;
    oss << "{\"item_id\":" << item_id << ",\"message\":\"" << random_string(100) << "\"}";
    return oss.str();
}

// Handle HTTP requests
void handle_request(http::request<http::string_body>&& req, http::response<http::string_body>& res) {
    if (req.method() != http::verb::get) {
        res = http::response<http::string_body>(http::status::bad_request, req.version());
        res.set(http::field::content_type, "text/plain");
        res.body() = "Invalid request-method";
        res.prepare_payload();
        return;
    }

    // Route: GET /
    if (req.target() == "/") {
        res = http::response<http::string_body>(http::status::ok, req.version());
        res.set(http::field::content_type, "application/json");
        res.body() = get_index_json();
        res.prepare_payload();
        return;
    }

    // Route: GET /items/{item_id}
    std::string target = std::string(req.target());
    std::string prefix = "/items/";
    if (target.substr(0, prefix.size()) == prefix) {
        std::string id_str = target.substr(prefix.size());
        try {
            int item_id = std::stoi(id_str);
            res = http::response<http::string_body>(http::status::ok, req.version());
            res.set(http::field::content_type, "application/json");
            res.body() = get_item_json(item_id);
            res.prepare_payload();
            return;
        } catch (...) {
            // Not a valid integer
        }
    }

    // Not found
    res = http::response<http::string_body>(http::status::not_found, req.version());
    res.set(http::field::content_type, "text/plain");
    res.body() = "Resource not found";
    res.prepare_payload();
}

void do_session(tcp::socket socket) {
    beast::flat_buffer buffer;
    try {
        http::request<http::string_body> req;
        http::read(socket, buffer, req);
        http::response<http::string_body> res;
        handle_request(std::move(req), res);
        http::write(socket, res);
    } catch (std::exception& e) {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
    // Gracefully close the socket
    beast::error_code ec;
    socket.shutdown(tcp::socket::shutdown_send, ec);
}

int main() {
    try {
        net::io_context ioc;
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};
        std::cout << "Server running on http://127.0.0.1:8080/\n";
        for (;;) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            std::thread([s = std::move(socket)]() mutable {
                            do_session(std::move(s));
                        }).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
    return 0;
}