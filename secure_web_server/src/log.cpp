#include "log.h"
#include <iostream>
#include <memory>  // For std::unique_ptr

// Static pointer to the ILogOutput interface
std::unique_ptr<ILogOutput> Logger::log_output = nullptr;

// Initialize the logger with specific output
void Logger::initialize(std::unique_ptr<ILogOutput> output) {
    log_output = std::move(output);
}

// Implement log_info
void Logger::log_info(const std::string &info_message) {
    log(info_message, LogLevel::INFO);
}

// Implement other log levels similarly
void Logger::log_request(const std::string &request) {
    log(request, LogLevel::INFO);  
}

void Logger::log_response(const std::string &response) {
    log(response, LogLevel::INFO);
}

void Logger::log_error(const std::string &error_message) {
    log(error_message, LogLevel::ERROR);
}

void Logger::log_warning(const std::string &warning_message) {
    log(warning_message, LogLevel::WARNING);
}

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

    if (log_output) {
        log_output->output_log(level_str + " " + message, level_str);  // Use the output interface
    }
}
