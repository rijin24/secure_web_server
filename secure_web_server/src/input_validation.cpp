#include "../include/input_validation.h"
#include <regex>
#include <iostream>
#include <unordered_map>
#include <exception>  // For exception handling

bool InputValidation::validate_name(const std::string &name) {
    try {
        const int MAX_NAME_LENGTH = 50;
        if (name.empty() || name.length() > MAX_NAME_LENGTH) {
            std::cerr << "Error: Name is too long or empty." << std::endl;
            return false;  // Invalid if name is too long or empty
        }

        // Allow letters, spaces, hyphens, and apostrophes
        std::regex name_pattern("^[A-Za-zÀ-ÿ'\\- ]+$");  // Includes accented characters, hyphens, and apostrophes
        if (!std::regex_match(name, name_pattern)) {
            std::cerr << "Error: Name can only contain letters, spaces, hyphens, and apostrophes." << std::endl;
            return false;
        }

        return true;
    } catch (const std::exception &e) {
        std::cerr << "Exception in validate_name: " << e.what() << std::endl;
        return false;
    }
}

bool InputValidation::validate_age(const std::string &age) {
    try {
        if (age.empty()) {
            std::cerr << "Error: Age cannot be empty." << std::endl;
            return false;
        }

        std::regex age_pattern("^\\d+$");  // Only digits are allowed
        if (!std::regex_match(age, age_pattern)) {
            std::cerr << "Error: Age must be a valid number." << std::endl;
            return false;
        }

        try {
            int age_num = std::stoi(age);
            if (age_num <= 0 || age_num > 120) {
                std::cerr << "Error: Age must be between 1 and 120." << std::endl;
                return false;
            }
        } catch (const std::exception &e) {
            std::cerr << "Invalid age value: " << e.what() << std::endl;
            return false;
        }

        return true;
    } catch (const std::exception &e) {
        std::cerr << "Exception in validate_age: " << e.what() << std::endl;
        return false;
    }
}

std::string InputValidation::sanitize_input(const std::string &input) {
    try {
        // Map of special characters to their corresponding HTML entities
        std::unordered_map<char, std::string> entity_map = {
            {'<', "&lt;"}, {'>', "&gt;"}, {'\"', "&quot;"}, {'\'', "&apos;"}, {'&', "&amp;"}
        };

        std::string sanitized = input;
        // Iterate over each character in the map and replace occurrences in the input string
        for (auto &entry : entity_map) {
            size_t pos = 0;
            while ((pos = sanitized.find(entry.first, pos)) != std::string::npos) {
                sanitized.replace(pos, 1, entry.second);
                pos += entry.second.length();  // Move past the inserted HTML entity
            }
        }
        return sanitized;
    } catch (const std::exception &e) {
        std::cerr << "Exception in sanitize_input: " << e.what() << std::endl;
        return "";  // Return an empty string if an error occurs during sanitization
    }
}
