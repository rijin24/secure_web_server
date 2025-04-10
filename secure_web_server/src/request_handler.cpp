#include "request_handler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

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

std::string RequestHandler::handle_post_request(const std::string &request) {
    // Parse POST data from the request body
    std::string post_data = parse_post_data(request);

    // Store or process the POST data as needed (for simplicity, we will just return it)
    std::string response_body = "Received POST data: " + post_data;

    return generate_http_response(response_body, "text/plain");
}

std::string RequestHandler::get_file_content(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file) {
        return ""; // Return empty if file not found
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string RequestHandler::generate_http_response(const std::string &body, const std::string &content_type) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "\r\n";
    response << body;
    return response.str();
}

std::string RequestHandler::parse_post_data(const std::string &request) {
    std::string body = request.substr(request.find("\r\n\r\n") + 4);  // Extract the body after the headers
    return body;
}
