#ifndef LOG_H
#define LOG_H

#include <string>

class Logger {
public:
    // Logs incoming requests
    static void log_request(const std::string &request);

    // Logs outgoing responses
    static void log_response(const std::string &response);
};

#endif // LOG_H
