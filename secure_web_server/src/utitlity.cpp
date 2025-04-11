#include "utility.h"

#include <regex>

namespace Utility {

    // Function to extract the value for a key (e.g., "name", "age") from the form data
    std::string extract_value(const std::string &data, const std::string &key) {
        size_t start_pos = data.find(key + "=");
        if (start_pos == std::string::npos) return "";  // Key not found

        start_pos += key.length() + 1;  // Skip past "key="
        size_t end_pos = data.find("&", start_pos);  // Find end of value (if exists)
        if (end_pos == std::string::npos) {
            end_pos = data.length();  // If no "&", it's the last value
        }

        return data.substr(start_pos, end_pos - start_pos);  // Extract the value
    }

    // Function to validate the name (simple example, alphanumeric and spaces)
    bool validate_name(const std::string &name) {
        std::regex name_regex("^[A-Za-z ]+$");  // Only letters and spaces allowed
        return std::regex_match(name, name_regex);
    }

    // Function to validate the age (only numeric values)
    bool validate_age(const std::string &age) {
        std::regex age_regex("^[0-9]+$");  // Only digits allowed
        return std::regex_match(age, age_regex);
    }

}
