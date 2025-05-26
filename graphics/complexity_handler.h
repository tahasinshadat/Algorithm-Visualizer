// complexity_handler.h
#pragma once
#include "types.h"
#include <utility>

class ComplexityHandler {
public:
    // returns {time, space} in Big-O notation string
    std::pair<std::string,std::string> getComplexity(Algorithm alg) const {
        switch (alg) {
            case Algorithm::BinarySearch: return {"O(log n)","O(1)"};
            // …fill out later…
            default: return {"–","–"};
        }
    }
};
