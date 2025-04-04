#include <iostream>
#include "server.h"

#define SERVER_PORT 8080

/**
 * @brief Entry point of the program.
 * 
 * Initializes and starts the server on the defined port.
 */
int main() {
    try {
        // Create server instance
        Server server(SERVER_PORT);

        // Start the server
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
