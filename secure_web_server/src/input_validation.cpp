#include "input_validation.h"
#include <regex>
#include <iostream>

bool InputValidation::validate_name(const std::string &name) {
    const int MAX_NAME_LENGTH = 50;
    if (name.empty() || name.length() > MAX_NAME_LENGTH) {
        return false;  // Invalid if name is too long or empty
    }

    // Allow HTML entities like "&lt;" and "&gt;"
    std::regex name_pattern("^[A-Za-z&;#]+$");  // Only letters and allowed HTML entities
    if (!std::regex_match(name, name_pattern)) {
        std::cerr << "Name can only contain letters and spaces." << std::endl;
        return false;
    }

    return true;
}

bool InputValidation::validate_age(const std::string &age) {
    if (age.empty()) {
        std::cerr << "Age cannot be empty." << std::endl;
        return false;
    }

    std::regex age_pattern("^\\d+$");  // Only digits are allowed
    if (!std::regex_match(age, age_pattern)) {
        std::cerr << "Age must be a valid number." << std::endl;
        return false;
    }

    int age_num = std::stoi(age);
    if (age_num <= 0 || age_num > 120) {
        std::cerr << "Age must be between 1 and 120." << std::endl;
        return false;
    }

    return true;
}

std::string InputValidation::sanitize_input(const std::string &input) {
    std::string sanitized = input;
    size_t pos;
    while ((pos = sanitized.find("<")) != std::string::npos) {
        sanitized.replace(pos, 1, "&lt;");
    }
    while ((pos = sanitized.find(">")) != std::string::npos) {
        sanitized.replace(pos, 1, "&gt;");
    }
    while ((pos = sanitized.find("\"")) != std::string::npos) {
        sanitized.replace(pos, 1, "&quot;");
    }
    while ((pos = sanitized.find("\'")) != std::string::npos) {
        sanitized.replace(pos, 1, "&apos;");
    }
    return sanitized;
}
