#include "server.h"

int main() {
    Server server(8080);  // Create a Server object with port 8080
    server.start();       // Start the server
    return 0;
}
