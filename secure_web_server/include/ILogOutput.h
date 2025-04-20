#ifndef ILOGOUTPUT_H
#define ILOGOUTPUT_H

#include <string>

class ILogOutput {
public:
    virtual void output_log(const std::string& message, const std::string& level) = 0;
    virtual ~ILogOutput() = default;
};

#endif // ILOGOUTPUT_H
