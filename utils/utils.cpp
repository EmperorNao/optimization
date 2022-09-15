#include "utils.h"


std::vector<std::string> split(std::string& s, char delim) {

    std::vector<std::string> output;

    uint64_t start = 0;
    uint64_t current = 0;
    while (current < s.size()) {

        if (s[current] == delim) {
            output.push_back(s.substr(start, current - start));
            start = current + 1;
        }

        ++current;

    }

    if (start != current) {
        output.push_back(s.substr(start, current - start));
    }

    return output;

}