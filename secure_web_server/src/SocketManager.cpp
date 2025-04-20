#include "../include/socket_manager.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cerrno>  // For error codes

SocketManager::SocketManager() {
    // Constructor can be used for initialization if needed
}

SocketManager::~SocketManager() {
    // Destructor can be used to clean up resources if necessary
}

/**
 * Creates a socket.
 * 
 * @return The socket file descriptor, or -1 if the creation failed.
 */
int SocketManager::create_socket() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        log_error("Failed to create socket. Error: " + std::string(strerror(errno)));
        return -1;
    }
    return server_socket;
}

/**
 * Binds the socket to a specified port.
 * 
 * @param server_socket The socket file descriptor.
 * @param port The port number to bind to.
 * @return True if binding is successful, otherwise false.
 */
bool SocketManager::bind_socket(int server_socket, int port) {
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        log_error("Bind failed: " + std::string(strerror(errno)));
        close(server_socket);  // Close the socket if bind fails
        return false;
    }
    return true;
}

/**
 * Listens for incoming client connections.
 * 
 * @param server_socket The socket file descriptor.
 * @return True if listening is successful, otherwise false.
 */
bool SocketManager::listen_socket(int server_socket) {
    if (listen(server_socket, 10) < 0) {
        log_error("Failed to listen on socket: " + std::string(strerror(errno)));
        close(server_socket);  // Close socket if listen fails
        return false;
    }
    return true;
}

/**
 * Accepts an incoming connection.
 * 
 * @param server_socket The server socket file descriptor.
 * @param client_addr The client address structure to fill.
 * @param client_len The length of the client address structure.
 * @return The client socket file descriptor, or -1 if the accept failed.
 */
int SocketManager::accept_connection(int server_socket, struct sockaddr_in& client_addr, socklen_t& client_len) {
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        log_error("Failed to accept connection. Error: " + std::string(strerror(errno)));
    }
    return client_socket;
}

/**
 * Logs error messages to the console.
 * 
 * @param error_message The error message to log.
 */
void SocketManager::log_error(const std::string& error_message) {
    std::cerr << "Error: " << error_message << std::endl;
}
