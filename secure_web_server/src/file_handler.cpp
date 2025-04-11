#include "file_handler.h"
#include <fstream>
#include <iostream>
#include <sstream>  // Include this for std::ostringstream

void FileHandler::save_to_file(const std::string &name, const std::string &age) {
    std::ofstream outfile("form_data.txt", std::ios::app);
    if (outfile.is_open()) {
        outfile << "Name: " << name << ", Age: " << age << std::endl;
        outfile.close();
    } else {
        std::cerr << "Error opening file to save data." << std::endl;
    }
}

std::string FileHandler::get_file_content(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file) {
        return "";  // Return empty if file cannot be opened
    }

    std::ostringstream buffer;  // Using std::ostringstream
    buffer << file.rdbuf();  // Reading file content into the buffer
    return buffer.str();  // Return content as string
}
