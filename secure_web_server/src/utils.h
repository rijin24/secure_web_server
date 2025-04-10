#ifndef UTILS_H
#define UTILS_H

#include <string>

// Function to extract value for a given key from the POST data
std::string extract_value(const std::string& data, const std::string& key);

#endif
