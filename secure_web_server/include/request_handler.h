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

    

    // Saves name and age data to a file
    static void save_to_file(const std::string &name, const std::string &age);
     
};

#endif
