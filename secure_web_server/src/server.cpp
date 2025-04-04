#include "server.h"

Server::Server(int port) : port(port) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Binding failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Listen failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    close(server_socket);
}

void Server::start() {
    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Failed to accept client!" << std::endl;
            continue;
        }

        std::cout << "Client connected!" << std::endl;
        close(client_socket); 
    }
}
