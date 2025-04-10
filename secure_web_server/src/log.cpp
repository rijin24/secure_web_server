#include "log.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

void Logger::log_request(const std::string &request) {
    std::ofstream log_file;
    log_file.open("server_log.txt", std::ios_base::app);  // Open log file in append mode
    
    if (log_file.is_open()) {
        // Get the current time for logging
        std::time_t current_time = std::time(nullptr);
        char timestamp[100];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
        
        log_file << "[" << timestamp << "] Request:\n" << request << "\n\n";
        log_file.close();
    } else {
        std::cerr << "Unable to open log file." << std::endl;
    }
}

void Logger::log_response(const std::string &response) {
    std::ofstream log_file;
    log_file.open("server_log.txt", std::ios_base::app);  // Open log file in append mode
    
    if (log_file.is_open()) {
        // Get the current time for logging
        std::time_t current_time = std::time(nullptr);
        char timestamp[100];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));

        log_file << "[" << timestamp << "] Response:\n" << response << "\n\n";
        log_file.close();
    } else {
        std::cerr << "Unable to open log file." << std::endl;
    }
}
