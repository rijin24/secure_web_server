#ifndef FILELOGOUTPUT_H
#define FILELOGOUTPUT_H

#include "log.h"
#include <fstream>  // For file output

// FileLogOutput that implements ILogOutput
class FileLogOutput : public ILogOutput {
public:
    // Constructor that accepts a file path to open for writing logs
    explicit FileLogOutput(const std::string& file_path);

    // Function to write the log message to the file
    void output_log(const std::string& message, const std::string& level) override;

    // Destructor to close the file when the object is destroyed
    virtual ~FileLogOutput(); 

private:
    std::ofstream log_file;  // File stream for logging
};

#endif // FILELOGOUTPUT_H
