#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "log.h" 

class Server {
public:
    Server(int port);  // Constructor that accepts a port number
    void start();      // Start the server
    static void *handle_client(void *arg);  // Handles each client in a new thread
    static std::string handle_post_request(const std::string &request); 
private:
    int port;          // Store the port number
    void run_server(int server_socket);  // Accepts incoming connections
};

#endif
