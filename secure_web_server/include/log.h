#ifndef LOG_H
#define LOG_H

#include <string>
#include <memory>  // For std::unique_ptr

// ILogOutput interface
class ILogOutput {
public:
    virtual void output_log(const std::string& message, const std::string& level) = 0;
    virtual ~ILogOutput() = default;
};

// Logger class that uses the ILogOutput interface
class Logger {
public:
    enum class LogLevel {
        INFO,
        ERROR,
        WARNING,
        DEBUG
    };

    // Method to initialize logger with specific output
    static void initialize(std::unique_ptr<ILogOutput> output);

    static void log_request(const std::string &request);
    static void log_response(const std::string &response);
    static void log_error(const std::string &error_message);
    static void log_warning(const std::string &warning_message);
    static void log_debug(const std::string &debug_message);
    static void log_info(const std::string &info_message);

private:
    static void log(const std::string &message, LogLevel level);
    static std::unique_ptr<ILogOutput> log_output;  // Log output interface pointer
};

#endif // LOG_H
