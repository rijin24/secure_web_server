#include "server.h"
#include <mutex>  // For thread-safe logging
#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <stdexcept>

// Mutex for logging thread safety
std::mutex log_mutex;

WebServer::WebServer(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(server_fd, 10) < 0) {
        throw std::runtime_error("Listen failed");
    }
}

WebServer::~WebServer() {
    close(server_fd);
}

void WebServer::start() {
    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket < 0) continue;
        std::cout << "Client connected, creating new thread..." << std::endl; // Debugging line
        std::thread(&WebServer::handleClient, this, client_socket).detach();
    }
}

void WebServer::handleClient(int client_socket) {
    char buffer[4096] = {0};
    read(client_socket, buffer, sizeof(buffer) - 1);
    std::string request(buffer);

    // Log the request
    logRequest(request);

    std::istringstream request_stream(request);
    std::string method, path;
    request_stream >> method >> path;

    if (method == "GET") {
        std::string file_path = "html" + path;
        if (file_path == "html/") file_path = "html/index.html";
        std::string content = getFileContent(file_path);
        if (content.empty()) send404(client_socket);
        else sendResponse(client_socket, content, "text/html");
    } 
    else if (method == "POST" && path == "/submit-registration") {
        handlePostRequest(client_socket, request);
    } 
    else {
        send404(client_socket);
    }

    close(client_socket);
}

void WebServer::sendResponse(int client_socket, const std::string& content, const std::string& content_type) {
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: " + content_type + "\r\nContent-Length: " + std::to_string(content.length()) + "\r\n\r\n" + content;
    send(client_socket, response.c_str(), response.length(), 0);
}

void WebServer::send404(int client_socket) {
    std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    send(client_socket, response.c_str(), response.length(), 0);
}

std::string WebServer::getFileContent(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) return "";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void WebServer::handlePostRequest(int client_socket, const std::string& request) {
    std::size_t body_pos = request.find("\r\n\r\n");
    if (body_pos == std::string::npos) return;

    std::string body = request.substr(body_pos + 4);

    // Log POST request data
    logRequest("[POST Data] " + body);

    std::ofstream file("registrations.txt", std::ios::app);
    file << body << "\n";
    file.close();

    sendResponse(client_socket, "<h1>Registration Successful</h1>", "text/html");
}

// Logging Function
void logRequest(const std::string& request) {
    std::lock_guard<std::mutex> lock(log_mutex);  // Ensure thread safety
    std::ofstream log_file("logs/server.log", std::ios::app);  // Append mode
    if (log_file) {
        log_file << "[Request] " << request << "\n";
    }
}
