#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <string>
#include "../include/server.h"
#include "../include/log.h"
#include "../include/ConsoleLogOutput.h"

// Function to apply resource limits to avoid excessive resource usage
void applyResourceLimits() {
    struct rlimit limit;

    // Limit CPU time to 60 seconds
    limit.rlim_cur = 60;
    limit.rlim_max = 60;
    if (setrlimit(RLIMIT_CPU, &limit) == -1) {
        Logger::log_error("Failed to set CPU limit");
    }

    // Limit number of open files to 32
    limit.rlim_cur = 32;
    limit.rlim_max = 64;
    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        Logger::log_error("Failed to set file descriptor limit");
    }
}

// Function to filter file access (sandbox directory)
bool isValidFilePath(const std::string& filepath) {
    // Prevent accessing files outside of /static
    if (filepath.find("/static") != std::string::npos) {
        return true;
    }

    // Log if someone tries to access outside /static
    Logger::log_warning("Blocked access to non-static path: " + filepath);
    return false;
}

int main() {
    // Initialize the logger with console output
    Logger::initialize(std::make_unique<ConsoleLogOutput>());

    // Apply resource limits to avoid excessive usage
    applyResourceLimits();

    // Check file path filtering before starting server
    std::string testFilePath = "/static/index.html"; // Change as needed
    if (!isValidFilePath(testFilePath)) {
        Logger::log_error("Invalid file path access attempt: " + testFilePath);
        return 1; // Exit if invalid path
    }

    Server server(8080);  // Create a Server object with port 8080
    server.start();       // Start the server
    return 0;
}
