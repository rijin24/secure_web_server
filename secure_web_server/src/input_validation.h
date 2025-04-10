#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

#include <string>

class InputValidation {
public:
    // Function to validate if the input is non-empty and has no special characters
    static bool validate_name(const std::string &name);

    // Function to validate if the age input is a valid number
    static bool validate_age(const std::string &age);

    // Function to sanitize input by replacing special HTML characters to prevent XSS
    static std::string sanitize_input(const std::string &input);
};

#endif
