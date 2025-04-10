#include "server.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include "request_handler.h"
#include "log.h"  // Include the logging functionality
#include "input_validation.h"  // Include the input validation header
#include "utils.h"

Server::Server(int port) : port(port) {}  // Constructor definition

// Function to sanitize the input to prevent XSS
std::string sanitize_input(const std::string &input) {
    std::string sanitized = input;
    // Replace special HTML characters to prevent XSS
    size_t pos;
    while ((pos = sanitized.find("<")) != std::string::npos) {
        sanitized.replace(pos, 1, "&lt;");
    }
    while ((pos = sanitized.find(">")) != std::string::npos) {
        sanitized.replace(pos, 1, "&gt;");
    }
    while ((pos = sanitized.find("\"")) != std::string::npos) {
        sanitized.replace(pos, 1, "&quot;");
    }
    while ((pos = sanitized.find("\'")) != std::string::npos) {
        sanitized.replace(pos, 1, "&apos;");
    }
    return sanitized;
}

void *Server::handle_client(void *arg) {
    int client_socket = *((int *)arg);
    delete (int *)arg;

    char buffer[4096] = {0};  
    read(client_socket, buffer, sizeof(buffer));

    std::string request(buffer);

    // Log the request (basic logging for now)
    Logger::log_request(request);

    // Check if it's a POST request
    std::string response;
    if (request.find("POST") != std::string::npos) {
        std::cout << "POST request received" << std::endl;
        // Handle POST request
        response = handle_post_request(request);
    } else {
        // Handle GET or other requests
        response = RequestHandler::handle_request(request);
    }

    // Log the response
    Logger::log_response(response);

    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);

    return nullptr;
}

std::string Server::handle_post_request(const std::string &request) {
    // Find the start of the data in the POST request
    std::string data = request.substr(request.find("\r\n\r\n") + 4);  // Form data starts after "\r\n\r\n"
    
    std::cout << "Received data: " << data << std::endl;
    
    // Extract name and age from the POST data (simple example)
    std::string name = extract_value(data, "name");
    std::string age = extract_value(data, "age");

    // Sanitize inputs before validation
    name = InputValidation::sanitize_input(name);
    age = InputValidation::sanitize_input(age);

    // Log the sanitized data
    std::cout << "Sanitized data: name=" << name << ", age=" << age << std::endl;

    // Validate the input data
    if (!InputValidation::validate_name(name)) {
        return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
               "<html><body><h1>Invalid Name</h1><p>The name entered is invalid.</p></body></html>";
    }

    if (!InputValidation::validate_age(age)) {
        return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
               "<html><body><h1>Invalid Age</h1><p>The age entered is invalid.</p></body></html>";
    }

    // If validation passed, process and respond
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n";
    response += "<html><body><h1>Form Submitted Successfully!</h1>";
    response += "<p>Data received: Name = " + name + ", Age = " + age + "</p></body></html>";

    return response;
}

void Server::run_server(int server_socket) {
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        
        if (client_socket < 0) {
            std::cerr << "Failed to accept connection." << std::endl;
            continue;
        }

        int *client_socket_ptr = new int(client_socket);

        pthread_t client_thread;
        if (pthread_create(&client_thread, nullptr, handle_client, client_socket_ptr) != 0) {
            std::cerr << "Failed to create thread." << std::endl;
        }

        pthread_detach(client_thread);
    }
}

void Server::start() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        exit(1);
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);  // Use the port member variable

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        exit(1);
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen." << std::endl;
        exit(1);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;

    run_server(server_socket);
}
