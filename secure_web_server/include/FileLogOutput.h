#ifndef FILELOGOUTPUT_H
#define FILELOGOUTPUT_H

#include "log.h"
#include <fstream>  // For file output

// FileLogOutput that implements ILogOutput
class FileLogOutput : public ILogOutput {
public:
    explicit FileLogOutput(const std::string& file_path);
    void output_log(const std::string& message, const std::string& level) override;

private:
    std::ofstream log_file;
};

#endif // FILELOGOUTPUT_H
