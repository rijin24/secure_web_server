#include "../include/utils.h"
#include <sstream>
#include <iostream>

std::string extract_value(const std::string& data, const std::string& key) {
    size_t start_pos = data.find(key + "=");
    if (start_pos == std::string::npos) {
        return "";  // Key not found
    }

    start_pos += key.length() + 1;  // Skip past the key and '='
    size_t end_pos = data.find("&", start_pos);

    if (end_pos == std::string::npos) {
        end_pos = data.length();  // No '&' means it's the last key-value pair
    }

    return data.substr(start_pos, end_pos - start_pos);
}
