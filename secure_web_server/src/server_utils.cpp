#include <iostream>
#include <sys/resource.h>
#include <string>
#include <algorithm>
#include <filesystem>
#include "../include/log.h"

namespace fs = std::filesystem;


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

bool isValidFilePath(const std::string& filepath) {
    // 1. Ensure the path starts with "/static" to limit access to the static folder
    if (filepath.find("/static") != 0) {
        Logger::log_warning("Blocked access to non-static path: " + filepath);
        return false;
    }

    // 2. Sanitize: Remove any instances of "../" from the file path (directory traversal attempts)
    std::string sanitized_path = filepath;
    size_t pos = sanitized_path.find("../");
    while (pos != std::string::npos) {
        sanitized_path.erase(pos, 3);  // Remove "../" (attempt at directory traversal)
        pos = sanitized_path.find("../");
    }

    // 3. Normalize the path (resolves symbolic links, relative paths, etc.)
    fs::path full_path = fs::canonical(fs::path(sanitized_path));

    // 4. Ensure the full path still lies within the allowed "static" directory
    if (full_path.string().find(fs::canonical("static").string()) != 0) {
        Logger::log_warning("Directory traversal attempt detected: " + filepath);
        return false;
    }

    // If all checks passed, the file path is valid
    return true;
}

