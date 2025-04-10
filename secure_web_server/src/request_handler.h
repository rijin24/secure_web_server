#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

class RequestHandler {
public:
    static std::string handle_request(const std::string &request);
    static std::string handle_post_request(const std::string &request);
private:
    static std::string get_file_content(const std::string &file_path);
    static std::string generate_http_response(const std::string &body, const std::string &content_type);
    static std::string parse_post_data(const std::string &request);
};

#endif
