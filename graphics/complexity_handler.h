// graphics/complexity_handler.h
#pragma once
#include "../utilities/types.h" // Includes global enums
#include <string>
#include <utility> // For std::pair
#include <map>

// Class is now in the global namespace

class ComplexityHandler {
public:
    ComplexityHandler();
    std::pair<std::string, std::string> getComplexity(Algorithm alg) const;

private:
    std::map<Algorithm, std::pair<std::string, std::string>> complexitiesMap; // Renamed to avoid conflict
    void initializeComplexities();
};
