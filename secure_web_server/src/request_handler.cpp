#include "../include/request_handler.h"
#include <sstream>
#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "../include/input_validation.h"   // For sanitization and validation
#include "../include/file_handler.h"       // For saving to file
#include "../include/log.h"                // Logging utility
#include "../include/utility.h"            // Correct Utility namespace with extract_value

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

        std::unique_ptr<FileHandler> file_handler = std::make_unique<FileHandler>();
        std::string file_path = "static" + path;
        std::string content = file_handler->get_file_content(file_path);

        if (content.empty()) {
            Logger::log_error("File not found: " + file_path);
            return generate_http_response("404 Not Found", "text/plain");
        }

        Logger::log_debug("Serving file: " + file_path);
        return generate_http_response(content, "text/html");

    } catch (const std::exception &e) {
        Logger::log_error("Error handling GET/POST request: " + std::string(e.what()));
        return generate_http_response("500 Internal Server Error", "text/plain");
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
            return generate_http_response_with_error("Invalid Name", "The name entered is invalid.");
        }

        if (!InputValidation::validate_age(age)) {
            Logger::log_warning("Invalid Age: " + age);
            return generate_http_response_with_error("Invalid Age", "The age entered is invalid.");
        }

        std::unique_ptr<FileHandler> file_handler = std::make_unique<FileHandler>();
        file_handler->save_to_file(name, age);

        Logger::log_info("Data saved successfully: Name=" + name + ", Age=" + age);
        return generate_http_response_with_success(name, age);

    } catch (const std::exception &e) {
        Logger::log_error("Error processing POST request: " + std::string(e.what()));
        return generate_http_response_with_error("Internal Server Error", "There was an error processing your request.");
    }
}

// Generate generic HTTP response
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
        Logger::log_error("Error generating HTTP response: " + std::string(e.what()));
        return generate_http_response_with_error("Internal Server Error", "There was an error generating the response.");
    }
}

// Generate success HTML response
std::string RequestHandler::generate_http_response_with_success(const std::string& name, const std::string& age) {
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
}

// Generate error HTML response
std::string RequestHandler::generate_http_response_with_error(const std::string& error_title, const std::string& error_message) {
    std::string response = "HTTP/1.1 400 Bad Request\r\n";
    response += "Content-Type: text/html\r\n\r\n";
    response += R"(
<!DOCTYPE html>
<html>
<head>
    <title>)" + error_title + R"(</title>
</head>
<body>
    <h1>)" + error_title + R"(</h1>
    <p>)" + error_message + R"(</p>
</body>
</html>
)";
    return response;
}
