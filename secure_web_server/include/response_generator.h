#ifndef RESPONSE_GENERATOR_H
#define RESPONSE_GENERATOR_H

#include <string>

class ResponseGenerator {
public:
    static std::string generate_http_response(const std::string& body, const std::string& content_type);
    static std::string generate_http_response_with_success(const std::string& name, const std::string& age);
    static std::string generate_http_response_with_error(const std::string& error_title, const std::string& error_message);
};

#endif // RESPONSE_GENERATOR_H
