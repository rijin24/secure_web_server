#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class SocketManager {
public:
    // Constructor and Destructor
    SocketManager();
    ~SocketManager();

    // Function to create a socket
    int create_socket();

    // Function to bind the socket to a port
    bool bind_socket(int server_socket, int port);

    // Function to start listening for incoming connections
    bool listen_socket(int server_socket);

    // Function to accept incoming client connections
    int accept_connection(int server_socket, struct sockaddr_in& client_addr, socklen_t& client_len);

private:
    // Error logging function to handle errors
    void log_error(const std::string& error_message);
};

#endif // SOCKETMANAGER_H
