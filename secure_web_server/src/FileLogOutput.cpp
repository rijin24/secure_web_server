#include "FileLogOutput.h"
#include <iostream>

FileLogOutput::FileLogOutput(const std::string& file_path) {
    log_file.open(file_path, std::ios::app);  // Open file in append mode
    if (!log_file.is_open()) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

void FileLogOutput::output_log(const std::string& message, const std::string& level) {
    if (log_file.is_open()) {
        log_file << message << std::endl;
    } else {
        std::cerr << "Failed to write log message to file. Log file not open." << std::endl;
    }
}

// Destructor to ensure the file is properly closed when the object is destroyed
FileLogOutput::~FileLogOutput() {
    if (log_file.is_open()) {
        log_file.close();
    }
}
