#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Server {
private:
    int server_socket;
    int port;
    struct sockaddr_in server_address;

public:
    Server(int port);
    ~Server();
    void start();
};

#endif // SERVER_H
