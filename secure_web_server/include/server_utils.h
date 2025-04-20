#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <string>

// Function to apply resource limits to avoid excessive resource usage
void applyResourceLimits();

// Function to validate file paths and ensure only valid files can be accessed
bool isValidFilePath(const std::string& filepath);

#endif // SERVER_UTILS_H
