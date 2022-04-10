#pragma once
#include <iostream>
#include <vector>

namespace HelperFunctions {

static inline std::vector<std::string> SplitString(const std::string &str, const std::string &delimiter = " ", const int max_elements = 0) {
    std::vector<std::string> tokens;
    std::string::size_type start_index = 0;
    while (true) {
        std::string::size_type next_index = str.find(delimiter, start_index);
        if (next_index == std::string::npos) {
            tokens.push_back(str.substr(start_index));
            break;
        } else {
            tokens.push_back(str.substr(start_index, next_index - start_index));
            start_index = next_index + delimiter.length();
        }
        if (max_elements > 0 && tokens.size() == max_elements - 1) {
            tokens.push_back(str.substr(start_index));
            break;
        }
    }

    return tokens;
}


void EraseWordFromString(std::string& mainWord, std::string wordToLookFor);
















}