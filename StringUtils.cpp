#include "stdafx.h"
#include "StringUtils.h"


std::vector<std::string> split_by_char(const std::string& text, char delimiter) 
{
    std::vector<std::string> result;
    std::stringstream ss(text);
    std::string token, tokenWithQuotes;

    while (std::getline(ss, token, delimiter)) 
    {
        if (token.empty())
        {
            result.push_back(token);
        }
        else if (token.front() == '"')
        {
            tokenWithQuotes = token;
        }
        else if (token.back() == '"')
        {
            result.push_back(tokenWithQuotes + ',' + token);
            tokenWithQuotes.clear();
        }
        else if(tokenWithQuotes.empty())
        {
            result.push_back(token);
        }
        else
        {
            tokenWithQuotes += ',' + token;
        }
    }
    return result;
}