#pragma once
#include <string>
#include <vector>

inline constexpr char DEFAULT_DELIMETER = '\t';

std::vector<std::string> SplitString(const std::string& string, char delimeter = DEFAULT_DELIMETER);
