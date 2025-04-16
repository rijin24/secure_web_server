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
void* Server::handle_client(void* arg) {
    int client_socket = *((int*)arg);
    delete (int*)arg;

    char buffer[4096] = {0};  

    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        Logger::log_error("Error reading from client socket.");
        std::cerr << "Error reading from client socket." << std::endl;
        close(client_socket);
        return nullptr;
    }

    std::string request(buffer);
    Logger::log_request(request);

    std::string response;
    try {
        if (request.find("POST") != std::string::npos) {
            Logger::log_info("POST request received.");
            response = RequestHandler::handle_post_request(request);
        } else {
            Logger::log_info("GET/Other request received.");
            response = RequestHandler::handle_request(request);
        }

        Logger::log_response(response);
        ssize_t bytes_sent = send(client_socket, response.c_str(), response.size(), 0);
        if (bytes_sent < 0) {
            Logger::log_error("Error sending response to client.");
            std::cerr << "Error sending response to client." << std::endl;
        }
    } catch (const std::exception& e) {
        Logger::log_error("Exception while processing the request: " + std::string(e.what()));
        std::cerr << "Exception while processing the request: " << e.what() << std::endl;
        response = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>Internal Server Error</h1></body></html>";
        send(client_socket, response.c_str(), response.size(), 0);
    }

    close(client_socket);
    return nullptr;
}

// Server start function
void Server::start() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        Logger::log_error("Failed to create socket.");
        std::cerr << "Failed to create socket." << std::endl;
        exit(1);
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        Logger::log_error("Bind failed: " + std::string(strerror(errno)));
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 10) < 0) {
        Logger::log_error("Failed to listen: " + std::string(strerror(errno)));
        std::cerr << "Failed to listen: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(1);
    }

    Logger::log_info("Server is listening on port " + std::to_string(port) + "...");

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            Logger::log_error("Failed to accept connection: " + std::string(strerror(errno)));
            std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
            continue;
        }

        int* client_socket_ptr = new int(client_socket);

        pthread_t client_thread;
        if (pthread_create(&client_thread, nullptr, handle_client, client_socket_ptr) != 0) {
            Logger::log_error("Failed to create thread: " + std::string(strerror(errno)));
            std::cerr << "Failed to create thread: " << strerror(errno) << std::endl;
            delete client_socket_ptr;
        } else {
            pthread_detach(client_thread);
        }
    }
}
