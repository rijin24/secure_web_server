#include "log.h"
#include <iostream>
#include <memory>  // For std::unique_ptr>

// Static vector to hold multiple ILogOutput interfaces
std::vector<std::unique_ptr<ILogOutput>> Logger::log_outputs;

void Logger::initialize(std::vector<std::unique_ptr<ILogOutput>> outputs) {
    // Move the passed vector into the log_outputs static member
    log_outputs = std::move(outputs);
}

// Implement log_info
void Logger::log_info(const std::string &info_message) {
    log(info_message, LogLevel::INFO);
}

// Implement log_request
void Logger::log_request(const std::string &request) {
    log(request, LogLevel::INFO);  
}

// Implement log_error
void Logger::log_error(const std::string &error_message) {
    log(error_message, LogLevel::ERROR);
}

// Implement log_warning
void Logger::log_warning(const std::string &warning_message) {
    log(warning_message, LogLevel::WARNING);
}

// Implement log_debug
void Logger::log_debug(const std::string &debug_message) {
    log(debug_message, LogLevel::DEBUG);
}

// Central log function
void Logger::log(const std::string &message, LogLevel level) {
    std::string level_str;
    switch (level) {
        case LogLevel::INFO:
            level_str = "[INFO]";
            break;
        case LogLevel::ERROR:
            level_str = "[ERROR]";
            break;
        case LogLevel::WARNING:
            level_str = "[WARNING]";
            break;
        case LogLevel::DEBUG:
            level_str = "[DEBUG]";
            break;
    }

    // Iterate over all log outputs and log the message
    for (auto& output : log_outputs) {
        output->output_log(level_str + " " + message, level_str);  // Use the output interface
    }
}
