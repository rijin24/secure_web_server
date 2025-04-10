#include "input_validation.h"
#include <regex>
#include <iostream>

bool InputValidation::validate_name(const std::string &name) {
        // Max length for the name
    const int MAX_NAME_LENGTH = 50;
    // Check if the name is not empty and contains only alphabets
      if (name.empty() || name.length() > MAX_NAME_LENGTH) {
            return false;  // Invalid if name is too long or empty
        }
    
    std::regex name_pattern("^[A-Za-z ]+$");
    if (!std::regex_match(name, name_pattern)) {
        std::cerr << "Name can only contain letters and spaces." << std::endl;
        return false;
    }

    return true;
}

bool InputValidation::validate_age(const std::string &age) {
    // Check if the age is a valid number
    if (age.empty()) {
        std::cerr << "Age cannot be empty." << std::endl;
        return false;
    }

    std::regex age_pattern("^\\d+$");  // Only digits are allowed
    if (!std::regex_match(age, age_pattern)) {
        std::cerr << "Age must be a valid number." << std::endl;
        return false;
    }

    // Optional: You can add a range check for age, e.g., between 1 and 120
    int age_num = std::stoi(age);
    if (age_num <= 0 || age_num > 120) {
        std::cerr << "Age must be between 1 and 120." << std::endl;
        return false;
    }

    return true;
}
