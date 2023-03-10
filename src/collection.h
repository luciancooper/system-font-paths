#ifndef COLLECTION_H
#define COLLECTION_H
#include <stdlib.h>

#include <algorithm>
#include <string>
#include <vector>

class StringVector : public std::vector<std::string> {
  public:
    // Override the push_back method to only add unique strings
    void push_back(const std::string& str) {
        // Check if the string already exists in the vector
        auto it = std::find(begin(), end(), str);
        if (it == end()) {
            // The string doesn't exist, so add it
            std::vector<std::string>::push_back(str);
        }
        // Otherwise, do nothing (i.e. don't add the string again)
    }

    // checks if this string list contains the given string
    bool contains(const std::string& str) const {
        for (const auto& s : *this) {
            if (s == str) {
                return true;
            }
        }
        return false;
    }
};

#endif
