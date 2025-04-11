#ifndef UTILITY_H
#define UTILITY_H

#include <string>

namespace Utility {
    std::string extract_value(const std::string &data, const std::string &key);
    bool validate_name(const std::string &name);
    bool validate_age(const std::string &age);
}

#endif
