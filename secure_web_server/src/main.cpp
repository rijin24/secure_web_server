#include "../include/server.h"
#include "../include/log.h"
#include "../include/ConsoleLogOutput.h"  

int main() {
    // Initialize the logger with console output
    Logger::initialize(std::make_unique<ConsoleLogOutput>());

    Server server(8080);  // Create a Server object with port 8080
    server.start();       // Start the server
    return 0;
}
