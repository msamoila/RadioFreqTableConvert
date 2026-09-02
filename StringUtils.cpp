#include "stdafx.h"
#include "StringUtils.h"


std::vector<std::string> split_by_char(const std::string& text, char delimiter) 
{
    std::vector<std::string> result;
    std::stringstream ss(text);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}