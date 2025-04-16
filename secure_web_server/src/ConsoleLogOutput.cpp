#include "ConsoleLogOutput.h"
#include <iostream>

void ConsoleLogOutput::output_log(const std::string& message, const std::string& level) {
    std::cout << level << " " << message << std::endl;
}
