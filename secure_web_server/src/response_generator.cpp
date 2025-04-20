#include "../include/response_generator.h"
#include "../include/log.h"
#include <sstream>
#include <exception>

std::string ResponseGenerator::generate_http_response(const std::string &body, const std::string &content_type) {
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

std::string ResponseGenerator::generate_http_response_with_success(const std::string& name, const std::string& age) {
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

std::string ResponseGenerator::generate_http_response_with_error(const std::string& error_title, const std::string& error_message) {
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
