#include "../include/server.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include "request_handler.h"
#include "../include/log.h"  // Include the logging functionality

// Constructor for Server
Server::Server(int port) : port(port) {}

// Thread function to handle client requests
void *Server::handle_client(void *arg) {
    int client_socket = *((int *)arg);
    delete (int *)arg;

    char buffer[4096] = {0};  
    read(client_socket, buffer, sizeof(buffer));

    std::string request(buffer);

    // Log the request
    Logger::log_request(request);

    // Check if it's a POST request
    std::string response;
    if (request.find("POST") != std::string::npos) {
        std::cout << "POST request received" << std::endl;
        // Handle POST request
        response = RequestHandler::handle_post_request(request);
    } else {
        // Handle GET or other requests
        response = RequestHandler::handle_request(request);
    }

    // Log the response
    Logger::log_response(response);

    // Send response to client
    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);

    return nullptr;
}

// Server start function
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

    // Accept and handle client connections in separate threads
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
