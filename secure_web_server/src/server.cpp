#include "../include/server.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include "request_handler.h"
#include "../include/log.h"
#include <memory>  // For smart pointers
#include "../include/socket_manager.h"

// Constructor for Server class
/**
 * Initializes the server with the given port and creates a SocketManager instance.
 *
 * @param port The port number the server will listen on.
 */
Server::Server(int port) 
    : port(port), socket_manager(std::make_unique<SocketManager>()) {}

/**
 * Thread function to handle client requests.
 * This function processes incoming requests and sends the response.
 * 
 * @param arg A pointer to the client socket passed when creating the thread.
 * @return nullptr After handling the client request, the function returns nullptr.
 */
void* Server::handle_client(void* arg) {
    // Use a unique_ptr to manage the client socket
    auto client_socket_ptr = std::unique_ptr<int>(reinterpret_cast<int*>(arg));
    char buffer[4096] = {0};  // Buffer to store incoming data from the client

    // Read data from the client socket
    ssize_t bytes_read = read(*client_socket_ptr, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        Logger::log_error("Error reading from client socket.");
        std::cerr << "Error reading from client socket." << std::endl;
        close(*client_socket_ptr);  // Close socket on error
        return nullptr;
    }

    std::string request(buffer);  // Convert buffer into a string for easier processing
    Logger::log_request(request);  // Log the incoming request

    std::string response;
    try {
        // Handle POST or GET requests
        if (request.find("POST") != std::string::npos) {
            Logger::log_info("POST request received.");
            response = RequestHandler::handle_post_request(request);  // Handle POST request
        } else {
            Logger::log_info("GET/Other request received.");
            response = RequestHandler::handle_request(request);  // Handle GET or other requests
        }

        // Extract and log the HTTP status line (e.g., "HTTP/1.1 200 OK")
        size_t end_of_status_line = response.find("\r\n");
        if (end_of_status_line != std::string::npos) {
            Logger::log_info("HTTP Response: " + response.substr(0, end_of_status_line));
        } else {
            Logger::log_info("HTTP Response: <Unable to extract status line>");
        }

        // Send the HTTP response back to the client
        ssize_t bytes_sent = send(*client_socket_ptr, response.c_str(), response.size(), 0);
        if (bytes_sent < 0) {
            Logger::log_error("Error sending response to client.");
            std::cerr << "Error sending response to client." << std::endl;
        }
    } catch (const std::exception& e) {
        // Log any exceptions that occur during request handling
        Logger::log_error("Exception while processing the request: " + std::string(e.what()));
        std::cerr << "Exception while processing the request: " << e.what() << std::endl;

        // Send a 500 Internal Server Error response in case of an exception
        response = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>Internal Server Error</h1></body></html>";
        Logger::log_info("HTTP Response: HTTP/1.1 500 Internal Server Error");
        send(*client_socket_ptr, response.c_str(), response.size(), 0);
    }

    // Close the client socket after processing the request
    close(*client_socket_ptr);
    return nullptr;
}

/**
 * The start function initializes the server, listens for incoming connections,
 * and creates a new thread for each incoming client connection.
 */
void Server::start() {
    // Create a server socket using SocketManager to handle socket creation
    int server_socket = socket_manager->create_socket();
    
    // Bind the socket to the server address
    if (!socket_manager->bind_socket(server_socket, port)) {
        std::cerr << "Failed to bind socket" << std::endl;
        exit(1);  // Exit if bind fails
    }

    // Start listening for incoming connections
    if (!socket_manager->listen_socket(server_socket)) {
        std::cerr << "Failed to listen" << std::endl;
        exit(1);  // Exit if listening fails
    }

    Logger::log_info("Server is listening on port " + std::to_string(port) + "...");

    // Main loop to accept and handle incoming client connections
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        // Accept a new client connection using SocketManager
        int client_socket = socket_manager->accept_connection(server_socket, client_addr, client_len);
        if (client_socket < 0) {
            Logger::log_error("Failed to accept connection.");
            continue;  // Continue accepting other connections if one fails
        }

        // Create a unique_ptr for the client socket and create a new thread
        auto client_socket_ptr = std::make_unique<int>(client_socket);

        pthread_t client_thread;
        if (pthread_create(&client_thread, nullptr, handle_client, client_socket_ptr.release()) != 0) {
            Logger::log_error("Failed to create thread.");
            std::cerr << "Failed to create thread" << std::endl;
        } else {
            pthread_detach(client_thread);  // Detach the thread to automatically clean up
        }
    }

    // Close the server socket after stopping
    close(server_socket);
}
