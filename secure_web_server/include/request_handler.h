#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

class RequestHandler {
public:
    // Handles incoming requests (POST, GET, etc.)
    static std::string handle_request(const std::string &request);

    // Handles POST requests and processes form data
    static std::string handle_post_request(const std::string &request);

private:
    // Reads file content
    static std::string get_file_content(const std::string &file_path);

    // Generates an HTTP response with the given body and content type
    static std::string generate_http_response(const std::string &body, const std::string &content_type);

    // Extracts value associated with a key from POST data
    static std::string extract_value(const std::string &data, const std::string &key);

    // Saves name and age data to a file
    static void save_to_file(const std::string &name, const std::string &age);
     static std::string generate_http_response_with_success(const std::string& name, const std::string& age);
    static std::string generate_http_response_with_error(const std::string& error_title, const std::string& error_message);
};

#endif
