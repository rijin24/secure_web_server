#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>

class WebServer {
public:
    WebServer(int port);
    ~WebServer();
    void start();

private:
    int server_fd;
    struct sockaddr_in server_address;
    std::mutex log_mutex;

    void handleClient(int client_socket);
    void sendResponse(int client_socket, const std::string& content, const std::string& content_type);
    void send404(int client_socket);
    std::string getFileContent(const std::string& file_path);
    void handlePostRequest(int client_socket, const std::string& request);
    
};
void logRequest(const std::string& request);
#endif
