#pragma once

#include <string>
#include <vector>
#include <cstdint>

void trim(std::string *s);
std::string join(const std::vector<std::string> &l, char c);
std::vector<std::string> split(const std::string &s, char delim);
