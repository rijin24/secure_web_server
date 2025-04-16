#ifndef CONSOLELOGOUTPUT_H
#define CONSOLELOGOUTPUT_H

#include "log.h"
#include <iostream>  // For console output

// ConsoleLogOutput that implements ILogOutput
class ConsoleLogOutput : public ILogOutput {
public:
    void output_log(const std::string& message, const std::string& level) override;
};

#endif // CONSOLELOGOUTPUT_H
