#include "utils.hpp"

std::vector<std::string> SplitString(const std::string& string, char delimeter) {
    std::vector<std::string> result;
    if (string.empty()) {
        return result;
    }
    size_t pos = 0;
    size_t new_pos = string.find(delimeter);
    while (new_pos != std::string::npos) {
        result.push_back(string.substr(pos, new_pos - pos));
        pos = new_pos + 1;
        new_pos = string.find(delimeter, pos);
    }
    result.push_back(string.substr(pos));
    return result;
}

