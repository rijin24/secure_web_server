#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "log.h"

class Server {
public:
    // Constructor that accepts a port number
    Server(int port);

    // Start the server
    void start();

    // Handles each client in a new thread
    static void* handle_client(void* arg);

    // Handles POST requests
    static std::string handle_post_request(const std::string& request);

private:
    // Store the port number
    int port;

    // Accepts incoming connections
    void run_server(int server_socket);
};

#endif // SERVER_H
