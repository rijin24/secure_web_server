#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <string>
#include "../include/server.h"
#include "../include/log.h"
#include "../include/ConsoleLogOutput.h"
#include "../include/FileLogOutput.h"  // Include the FileLogOutput header

// Function to apply resource limits to avoid excessive resource usage
void applyResourceLimits() {
    struct rlimit limit;

    // Set a limit on CPU time (in seconds) to avoid server running indefinitely or overloading the CPU.
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
bool isValidFilePath(const std::string& filepath) {
    if (filepath.find("/static") != std::string::npos && filepath.find("..") == std::string::npos) {
        return true;
    }

    // Log a warning if an invalid path is detected
    Logger::log_warning("Blocked access to non-static path: " + filepath);
    return false;
}

int main() {
    // Initialize the logger to output logs to both the console and the file "server_log.txt".
    // Create a vector of log outputs and pass it to the initialize function.
    std::vector<std::unique_ptr<ILogOutput>> log_outputs;
    log_outputs.push_back(std::make_unique<ConsoleLogOutput>());
    log_outputs.push_back(std::make_unique<FileLogOutput>("server_log.txt"));

    // Initialize the logger with both outputs
    Logger::initialize(std::move(log_outputs));

    // Apply resource limits to prevent excessive resource usage by the server.
    applyResourceLimits();

    // Test a specific file path before the server starts to ensure it's valid.
    std::string testFilePath = "/static/index.html"; 
    if (!isValidFilePath(testFilePath)) {
        // Log and exit if the file path is invalid
        Logger::log_error("Invalid file path access attempt: " + testFilePath);
        return 1; // Exit if invalid path
    }

    // Print a message to indicate the server is starting
    std::cout << "Server started on port 8080..." << std::endl;

    // Create and start the server
    Server server(8080);  
    server.start();       

    return 0;
}
