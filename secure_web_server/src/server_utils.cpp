#include "server_utils.h"
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include "../include/log.h"

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
