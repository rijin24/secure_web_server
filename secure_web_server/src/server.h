#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>

class Server {
private:
    int server_socket;
   struct sockaddr_in server_address;
    std::vector<std::thread> client_threads;
    std::mutex mtx;

    void handleClient(int client_socket);  // Function to handle client communication

public:
    explicit Server(int port);
    ~Server();
    
    void start();
};

#endif // SERVER_H