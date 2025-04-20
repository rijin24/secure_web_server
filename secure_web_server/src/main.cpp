#include <iostream>
#include <vector>
#include <memory>
#include "../include/server.h"
#include "../include/log.h"
#include "../include/ConsoleLogOutput.h"
#include "../include/FileLogOutput.h"  // Include the FileLogOutput header
#include "server_utils.h"  // Include the header for utility functions

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
