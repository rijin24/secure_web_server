#include "../include/input_validation.h"
#include <regex>
#include <iostream>
#include <unordered_map>
#include <exception>

/**
 * @brief Validates a user's name based on length and allowed characters.
 * 
 * This function ensures the name is not empty, does not exceed the maximum allowed length,
 * and contains only alphabetical characters (including accented letters), spaces, hyphens, and apostrophes.
 * 
 * @param name The input string representing the name.
 * @return true if the name is valid.
 * @return false if the name is invalid.
 */
bool InputValidation::validate_name(const std::string &name) {
    try {
        const int MAX_NAME_LENGTH = 50;

        if (name.empty() || name.length() > MAX_NAME_LENGTH) {
            std::cerr << "Error: Name is too long or empty." << std::endl;
            return false;
        }

        std::regex name_pattern("^[A-Za-zÀ-ÿ'\\- ]+$");
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

/**
 * @brief Validates the user's age input.
 * 
 * This function checks that the input age is a positive integer without leading zeros
 * and falls within a realistic human age range (1–120).
 * 
 * @param age The input string representing age.
 * @return true if the age is valid.
 * @return false if the age is invalid.
 */
bool InputValidation::validate_age(const std::string &age) {
    try {
        if (age.empty()) {
            std::cerr << "Error: Age cannot be empty." << std::endl;
            return false;
        }

        std::regex age_pattern("^\\d+$");
        if (!std::regex_match(age, age_pattern)) {
            std::cerr << "Error: Age must be a valid number." << std::endl;
            return false;
        }

        // Reject leading zeroes unless the age is a single digit '0'
        if (age.length() > 1 && age[0] == '0') {
            std::cerr << "Error: Age should not contain leading zeros." << std::endl;
            return false;
        }

        int age_num = std::stoi(age);
        if (age_num <= 0 || age_num > 120) {
            std::cerr << "Error: Age must be between 1 and 120." << std::endl;
            return false;
        }

        return true;
    } catch (const std::exception &e) {
        std::cerr << "Exception in validate_age: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Sanitizes input by escaping HTML special characters.
 * 
 * This is a basic form of XSS prevention by converting special characters
 * to their corresponding HTML-safe entity representations.
 * 
 * Characters replaced:
 * - `<` becomes `&lt;`
 * - `>` becomes `&gt;`
 * - `"` becomes `&quot;`
 * - `'` becomes `&apos;`
 * - `&` becomes `&amp;`
 * 
 * @param input The raw user input string.
 * @return std::string A sanitized version of the input safe for HTML output.
 */
std::string InputValidation::sanitize_input(const std::string &input) {
    try {
        std::unordered_map<char, std::string> entity_map = {
            {'<', "&lt;"}, {'>', "&gt;"}, {'\"', "&quot;"},
            {'\'', "&apos;"}, {'&', "&amp;"}
        };

        std::string sanitized;
        sanitized.reserve(input.length());

        for (char ch : input) {
            auto it = entity_map.find(ch);
            if (it != entity_map.end()) {
                sanitized += it->second;
            } else {
                sanitized += ch;
            }
        }

        return sanitized;
    } catch (const std::exception &e) {
        std::cerr << "Exception in sanitize_input: " << e.what() << std::endl;
        return "";
    }
}
