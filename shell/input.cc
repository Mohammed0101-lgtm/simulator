//
// input.cc
//

#include "input.h"
#include "config.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


std::string Input_Handler::read_line() const {
    std::string line;
    std::getline(std::cin, line);
    if (line.length() > 0 && line.back() == '\n')
        line.pop_back();  // Remove the trailing newline character

    if (line.empty())
        return "";

    // Remove leading and trailing whitespace
    line.erase(line.begin(),
               std::find_if(line.begin(), line.end(), [](unsigned char ch) { return !std::isspace(ch); }));

    return line;
}

// tokenize the input string based on whitespace and special characters
std::vector<std::string> Input_Handler::tokenize(const std::string& str) const {
    std::vector<std::string> tokens;
    std::size_t              start = str.find_first_not_of(_TOK_DELIM);
    std::size_t              end   = 0;

    while ((end = str.find_first_of(_TOK_DELIM, start)) != std::string::npos)
    {
        if (end > start)
            tokens.emplace_back(str.substr(start, end - start));

        start = str.find_first_not_of(_TOK_DELIM, end);
    }

    if (start != std::string::npos)
        tokens.emplace_back(str.substr(start));

    return tokens;
}