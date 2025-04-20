#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <string>
#include "../include/server.h"
#include "../include/log.h"
#include "../include/ConsoleLogOutput.h"

// Function to apply resource limits to avoid excessive resource usage
// This helps in ensuring that the server doesn't over-consume system resources.
void applyResourceLimits() {
    struct rlimit limit;

    // Set a limit on CPU time (in seconds) to avoid server running indefinitely or overloading the CPU.
    // This helps in preventing denial of service (DoS) attacks or unintentional infinite loops.
    limit.rlim_cur = 60; // Soft limit: can be increased up to rlim_max
    limit.rlim_max = 60; // Hard limit: the upper limit of resource usage
    if (setrlimit(RLIMIT_CPU, &limit) == -1) {
        // Log error if setting the CPU limit fails
        perror("Failed to set CPU limit");
        Logger::log_error("Failed to set CPU limit");
    }

    // Limit the number of open file descriptors to 32 to avoid overloading the system with too many connections.
    limit.rlim_cur = 32;  // Soft limit: can be increased up to rlim_max
    limit.rlim_max = 64;  // Hard limit: upper bound of open files
    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        // Log error if setting the file descriptor limit fails
        perror("Failed to set file descriptor limit");
        Logger::log_error("Failed to set file descriptor limit");
    }
}

// Function to validate file paths and ensure only valid files can be accessed
// This is crucial for preventing unauthorized file access or path traversal attacks.
bool isValidFilePath(const std::string& filepath) {
    // Check if the file path is within the allowed /static directory and does not contain path traversal patterns like "..".
    if (filepath.find("/static") != std::string::npos && filepath.find("..") == std::string::npos) {
        return true;
    }

    // Log a warning if an invalid path is detected to aid in monitoring and debugging.
    Logger::log_warning("Blocked access to non-static path: " + filepath);
    return false;
}

int main() {
    // Initialize the logger to output logs to the console.
    // A logger is essential for monitoring the server’s health and error reporting.
    Logger::initialize(std::make_unique<ConsoleLogOutput>());

    // Apply resource limits to prevent excessive resource usage by the server.
    applyResourceLimits();

    // Test a specific file path before the server starts to ensure it's valid.
    std::string testFilePath = "/static/index.html"; // Sample test file, change as necessary
    if (!isValidFilePath(testFilePath)) {
        // Log and exit if the file path is invalid.
        Logger::log_error("Invalid file path access attempt: " + testFilePath);
        return 1; // Exit if invalid path to prevent further issues
    }

    // Print a message to indicate the server is starting and listening on port 8080.
    // This gives feedback when running the server in the terminal or logs.
    std::cout << "Server started on port 8080..." << std::endl;

    // Create a Server object, specifying the port on which the server should listen.
    // This will initiate the server listening and handling incoming HTTP requests.
    Server server(8080);  

    // Start the server to begin accepting client connections and handling requests.
    server.start();       

    return 0;
}
