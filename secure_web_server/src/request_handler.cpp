#include "request_handler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include "input_validation.h"  // Include input validation logic

// Function to handle GET and POST requests
std::string RequestHandler::handle_request(const std::string &request) {
    std::istringstream request_stream(request);
    std::string method, path, http_version;

    request_stream >> method >> path >> http_version;

    if (method == "POST") {
        return handle_post_request(request);  // Handle POST request
    }

    // Default to handling GET requests
    if (path == "/") {
        path = "/index.html";
    }

    std::string file_path = "static" + path;
    std::string content = get_file_content(file_path);

    if (content.empty()) {
        return generate_http_response("404 Not Found", "text/plain");
    }

    return generate_http_response(content, "text/html");
}

// Handle POST request and sanitize data
std::string RequestHandler::handle_post_request(const std::string &request) {
    // Extract the data from the POST request
    std::string data = request.substr(request.find("\r\n\r\n") + 4);  // Form data starts after "\r\n\r\n"

    std::cout << "Received data: " << data << std::endl;

    // Extract name and age from the POST data
    std::string name = extract_value(data, "name");
    std::string age = extract_value(data, "age");

    // Sanitize inputs before validation
    name = InputValidation::sanitize_input(name);
    age = InputValidation::sanitize_input(age);

    // Log the sanitized data
    std::cout << "Sanitized data: name=" << name << ", age=" << age << std::endl;

    // Validate the input data
    if (!InputValidation::validate_name(name)) {
        return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
               "<html><body><h1>Invalid Name</h1><p>The name entered is invalid.</p></body></html>";
    }

    if (!InputValidation::validate_age(age)) {
        return "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
               "<html><body><h1>Invalid Age</h1><p>The age entered is invalid.</p></body></html>";
    }

    // Save name and age to a file
    save_to_file(name, age);

    // If validation passed, process and respond
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n";
    response += "<html><body><h1>Form Submitted Successfully!</h1>";
    response += "<p>Data received: Name = " + name + ", Age = " + age + "</p></body></html>";

    return response;
}

// Save name and age to a file
void RequestHandler::save_to_file(const std::string &name, const std::string &age) {
    std::ofstream outfile("form_data.txt", std::ios::app);
    if (outfile.is_open()) {
        outfile << "Name: " << name << ", Age: " << age << std::endl;
        outfile.close();
    } else {
        std::cerr << "Error opening file to save data." << std::endl;
    }
}

// Function to extract values from POST data
std::string RequestHandler::extract_value(const std::string &data, const std::string &key) {
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
}

// Function to read the content of a file
std::string RequestHandler::get_file_content(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file) {
        return ""; // Return empty if file not found
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to generate HTTP response
std::string RequestHandler::generate_http_response(const std::string &body, const std::string &content_type) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "\r\n";
    response << body;
    return response.str();
}
