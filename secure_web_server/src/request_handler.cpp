#include "../include/request_handler.h"
#include <sstream>
#include <iostream>
#include <string>
#include "../include/input_validation.h"  // Include input validation logic
#include "../include/file_handler.h"  // Include FileHandler class
#include <memory>  // Include for smart pointers
#include <exception>  // For exception handling
#include "../include/log.h"  // Include the logging functionality

// Function to handle GET and POST requests
std::string RequestHandler::handle_request(const std::string &request) {
    try {
        std::istringstream request_stream(request);
        std::string method, path, http_version;

        request_stream >> method >> path >> http_version;

        // Log the incoming request
        Logger::log_request(request);  // Log the request for auditing

        // Handle POST request separately
        if (method == "POST") {
            return handle_post_request(request);  // Handle POST request
        }

        // Default to handling GET requests
        if (path == "/") {
            path = "/index.html";  // Default to index.html if root is requested
        }

        // Use smart pointer for FileHandler
        std::unique_ptr<FileHandler> file_handler = std::make_unique<FileHandler>();
        std::string file_path = "static" + path;
        std::string content = file_handler->get_file_content(file_path);

        if (content.empty()) {
            // Log missing file error
            Logger::log_error("File not found: " + file_path);
            return generate_http_response("404 Not Found", "text/plain");
        }

        // Log the file served
        Logger::log_debug("Serving file: " + file_path);
        return generate_http_response(content, "text/html");
    } catch (const std::exception &e) {
        // Log error during request handling
        Logger::log_error("Error handling GET/POST request: " + std::string(e.what()));
        return generate_http_response("500 Internal Server Error", "text/plain");
    }
}

// Handle POST request and sanitize data
std::string RequestHandler::handle_post_request(const std::string &request) {
    try {
        // Extract the data from the POST request
        std::string data = request.substr(request.find("\r\n\r\n") + 4);  // Form data starts after "\r\n\r\n"

        // Log received POST data for debugging
        Logger::log_debug("Received POST data: " + data);

        // Extract name and age from the POST data
        std::string name = extract_value(data, "name");
        std::string age = extract_value(data, "age");

        // Sanitize inputs before validation
        name = InputValidation::sanitize_input(name);
        age = InputValidation::sanitize_input(age);

        // Log sanitized data
        Logger::log_debug("Sanitized data: name=" + name + ", age=" + age);

        // Validate the input data
        if (!InputValidation::validate_name(name)) {
            // Log invalid name error
            Logger::log_warning("Invalid Name: " + name);
            return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>Invalid Name</h1><p>The name entered is invalid.</p></body></html>";
        }

        if (!InputValidation::validate_age(age)) {
            // Log invalid age error
            Logger::log_warning("Invalid Age: " + age);
            return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
                   "<html><body><h1>Invalid Age</h1><p>The age entered is invalid.</p></body></html>";
        }

        // Use smart pointer for FileHandler to save the data
        std::unique_ptr<FileHandler> file_handler = std::make_unique<FileHandler>();
        file_handler->save_to_file(name, age);

        // Log success
        Logger::log_info("Data saved successfully: Name=" + name + ", Age=" + age);

        // If validation passed, process and respond
std::string response = "HTTP/1.1 200 OK\r\n";
response += "Content-Type: text/html\r\n\r\n";
response += R"(
<!DOCTYPE html>
<html>
<head>
    <title>Submission Success</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(to right, #16a085, #2ecc71);
            color: #ffffff;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .message-box {
            background-color: rgba(0, 0, 0, 0.6);
            padding: 40px;
            border-radius: 10px;
            text-align: center;
            box-shadow: 0 8px 16px rgba(0,0,0,0.3);
        }
        h1 {
            margin-bottom: 20px;
        }
    </style>
</head>
<body>
    <div class='message-box'>
        <h1>Form Submitted Successfully!</h1>
        <p>Data received: Name = )" + name + R"(, Age = )" + age + R"(</p>
    </div>
</body>
</html>
)";


        return response;
    } catch (const std::exception &e) {
        // Log error during POST request processing
        Logger::log_error("Error processing POST request: " + std::string(e.what()));
        return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
               "<html><body><h1>Internal Server Error</h1><p>There was an error processing your request.</p></body></html>";
    }
}

// Function to extract values from POST data
std::string RequestHandler::extract_value(const std::string &data, const std::string &key) {
    try {
        size_t start_pos = data.find(key + "=");
        if (start_pos != std::string::npos) {
            start_pos += key.length() + 1;
            size_t end_pos = data.find("&", start_pos);
            if (end_pos == std::string::npos) {
                end_pos = data.length();
            }
            return data.substr(start_pos, end_pos - start_pos);
        }
        return "";
    } catch (const std::exception &e) {
        // Log error during value extraction
        Logger::log_error("Error extracting value for key '" + key + "': " + std::string(e.what()));
        return "";
    }
}

// Function to generate HTTP response
std::string RequestHandler::generate_http_response(const std::string &body, const std::string &content_type) {
    try {
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: " << content_type << "\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "\r\n";
        response << body;
        return response.str();
    } catch (const std::exception &e) {
        // Log error during HTTP response generation
        Logger::log_error("Error generating HTTP response: " + std::string(e.what()));
        return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n"
               "<html><body><h1>Internal Server Error</h1><p>There was an error generating the response.</p></body></html>";
    }
}
