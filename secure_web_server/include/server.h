#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <memory>
#include "log.h"
#include "socket_manager.h"
#include "request_handler.h"

class Server {
public:
    // Constructor that accepts a port number
    Server(int port);

    // Start the server
    void start();

    // Handles each client in a new thread
    static void* handle_client(void* arg);

private:
    // Store the port number
    int port;

    // SocketManager object to handle socket operations
    std::unique_ptr<SocketManager> socket_manager;

    // Accepts incoming connections and processes them
    void run_server(int server_socket);
};

#endif // SERVER_H
