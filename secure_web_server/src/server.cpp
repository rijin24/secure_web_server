#include "server.h"

/**
 * @brief Constructor to initialize the server.
 * @param port The port number the server will listen on.
 */
Server::Server(int port) {
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        throw std::runtime_error("Socket creation failed!");
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Bind socket to the address
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error("Binding failed!");
    }

    // Start listening for incoming connections
    if (listen(server_socket, 10) < 0) {
        throw std::runtime_error("Listen failed!");
    }

    std::cout << "Server started on port " << port << std::endl;
}

/**
 * @brief Destructor to close the server socket and clean up resources.
 */
Server::~Server() {
    close(server_socket);
    std::cout << "Server shut down." << std::endl;
}

/**
 * @brief Function to handle communication with a connected client.
 * @param client_socket The socket descriptor for the client.
 */
void Server::handleClient(int client_socket) {
    char buffer[1024] = {0};

    // Read data from client
    int bytes_received = read(client_socket, buffer, sizeof(buffer));
    if (bytes_received > 0) {
        std::cout << "Received: " << buffer << std::endl;

        // Send response
        std::string response = "HTTP/1.1 200 OK\nContent-Length: 13\n\nHello, World!";
        send(client_socket, response.c_str(), response.length(), 0);
    }

    close(client_socket);
}

/**
 * @brief Starts the server to accept client connections.
 */
void Server::start() {
    while (true) {
        // Accept a new client connection
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Failed to accept client!" << std::endl;
            continue;
        }

        std::cout << "Client connected!" << std::endl;

        // Handle the client in a separate thread
        std::lock_guard<std::mutex> lock(mtx);
        client_threads.emplace_back(&Server::handleClient, this, client_socket);
    }

    // Join all client threads (cleanup)
    for (auto& thread : client_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
