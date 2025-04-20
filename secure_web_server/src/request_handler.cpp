#include "../include/request_handler.h"
#include <sstream>
#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "../include/input_validation.h"   // For sanitization and validation
#include "../include/file_handler.h"       // For saving to file
#include "../include/log.h"                // Logging utility
#include "../include/utility.h"            // For extract_value
#include "../include/response_generator.h" // For HTTP response generation
#include "../include/server_utils.h"       // For isValidFilePath()

using namespace Utility;  // Use Utility::extract_value directly

// Handle GET and POST requests
std::string RequestHandler::handle_request(const std::string &request) {
    try {
        std::istringstream request_stream(request);
        std::string method, path, http_version;

        request_stream >> method >> path >> http_version;
        Logger::log_request(request);  // Audit log

        if (method == "POST") {
            return handle_post_request(request);  // Delegate to POST handler
        }

        if (path == "/") {
            path = "/index.html";  // Default page
        }

        std::string file_path = "static" + path;

        // Validate file path
       if (file_path.find("static") != std::string::npos && file_path.find("..") == std::string::npos) 
    Logger::log_debug("Valid file path: " + file_path);
else {
    Logger::log_warning("Blocked access to non-static path: " + file_path);
    return ResponseGenerator::generate_http_response("403 Forbidden", "text/plain");
}


        std::unique_ptr<FileHandler> file_handler = std::make_unique<FileHandler>();
        std::string content = file_handler->get_file_content(file_path);

        if (content.empty()) {
            Logger::log_error("File not found: " + file_path);
            return ResponseGenerator::generate_http_response("404 Not Found", "text/plain");
        }

        Logger::log_debug("Serving file: " + file_path);
        return ResponseGenerator::generate_http_response(content, "text/html");

    } catch (const std::exception &e) {
        Logger::log_error("Error handling GET/POST request: " + std::string(e.what()));
        return ResponseGenerator::generate_http_response("500 Internal Server Error", "text/plain");
    }
}

// Handle POST requests
std::string RequestHandler::handle_post_request(const std::string &request) {
    try {
        std::string data = request.substr(request.find("\r\n\r\n") + 4);
        Logger::log_debug("Received POST data: " + data);

        std::string name = extract_value(data, "name");
        std::string age = extract_value(data, "age");

        name = InputValidation::sanitize_input(name);
        age = InputValidation::sanitize_input(age);

        Logger::log_debug("Sanitized data: name=" + name + ", age=" + age);

        if (!InputValidation::validate_name(name)) {
            Logger::log_warning("Invalid Name: " + name);
            return ResponseGenerator::generate_http_response_with_error("Invalid Name", "The name entered is invalid.");
        }

        if (!InputValidation::validate_age(age)) {
            Logger::log_warning("Invalid Age: " + age);
            return ResponseGenerator::generate_http_response_with_error("Invalid Age", "The age entered is invalid.");
        }

        std::unique_ptr<FileHandler> file_handler = std::make_unique<FileHandler>();
        file_handler->save_to_file(name, age);

        Logger::log_info("Data saved successfully: Name=" + name + ", Age=" + age);
        return ResponseGenerator::generate_http_response_with_success(name, age);

    } catch (const std::exception &e) {
        Logger::log_error("Error processing POST request: " + std::string(e.what()));
        return ResponseGenerator::generate_http_response_with_error("Internal Server Error", "There was an error processing your request.");
    }
}
