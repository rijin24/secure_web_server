#include "FileLogOutput.h"
#include <iostream>

FileLogOutput::FileLogOutput(const std::string& file_path) {
    log_file.open(file_path, std::ios::app);
    if (!log_file) {
        std::cerr << "Error opening log file!" << std::endl;
    }
}

void FileLogOutput::output_log(const std::string& message, const std::string& level) {
    if (log_file) {
        log_file << level << " " << message << std::endl;
    } else {
        std::cerr << "Failed to write log message to file." << std::endl;
    }
}
