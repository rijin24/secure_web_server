#ifndef ILOGOUTPUT_H
#define ILOGOUTPUT_H

#include <string>

class ILogOutput {
public:
    virtual void write(const std::string& message) = 0;
    virtual ~ILogOutput() = default;
};

#endif // ILOGOUTPUT_H
