#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

class FileHandler {
public:
    // Method to get file content
    std::string get_file_content(const std::string &file_path);

    // Method to save data to a file
    void save_to_file(const std::string &name, const std::string &age);
};

#endif // FILE_HANDLER_H
