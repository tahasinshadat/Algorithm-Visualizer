// graphics/visualization_handler.cpp
#include "visualization_handler.h"
#include <iostream> // Optional for debugging

// Using declarations for std types in .cpp file
using std::string;
using std::vector;
using std::set; // From types.h DS_TO_ALGORITHMS_MAP
// No custom namespace for av::VisualizationHandler

VisualizationHandler::VisualizationHandler()
    : currentDS(DataStructure::Array), // Default to Array
      currentAlg(Algorithm::None),
      speedMs(300) {}

void VisualizationHandler::setDataStructure(DataStructure ds) {
    if (ds != DataStructure::None) { // User cannot select "None"
        currentDS = ds;
        currentAlg = Algorithm::None;
    }
}

void VisualizationHandler::setAlgorithm(Algorithm alg) {
    DataStructure mapKey = (currentDS == DataStructure::Vector) ? DataStructure::Array : currentDS;
    auto it = DS_TO_ALGORITHMS_MAP.find(mapKey);
    if (it != DS_TO_ALGORITHMS_MAP.end()) {
        if (it->second.count(alg) || alg == Algorithm::None) {
            currentAlg = alg;
        } else {
            currentAlg = Algorithm::None;
        }
    } else { // No algorithms defined for this DS (or its alias)
        currentAlg = Algorithm::None;
    }
}

DataStructure VisualizationHandler::getCurrentDataStructure() const {
    return currentDS;
}

string VisualizationHandler::getCurrentDataStructureDisplayName() const {
    if (currentDS == DataStructure::Array || currentDS == DataStructure::Vector) {
        return ARRAY_VECTOR_DISPLAY_NAME;
    }
    if (static_cast<size_t>(currentDS) < DATA_STRUCTURE_NAMES.size()) {
        // Accessing global DATA_STRUCTURE_NAMES
        return DATA_STRUCTURE_NAMES[static_cast<int>(currentDS)];
    }
    return "Unknown DS";
}

Algorithm VisualizationHandler::getCurrentAlgorithm() const {
    return currentAlg;
}

vector<Algorithm> VisualizationHandler::getAvailableAlgorithms() const {
    vector<Algorithm> available;
    DataStructure mapKey = (currentDS == DataStructure::Vector) ? DataStructure::Array : currentDS;
    
    auto it = DS_TO_ALGORITHMS_MAP.find(mapKey); // Accessing global DS_TO_ALGORITHMS_MAP
    if (it != DS_TO_ALGORITHMS_MAP.end()) {
        for (Algorithm alg : it->second) {
            available.push_back(alg);
        }
    }
    return available;
}

int VisualizationHandler::getSpeedMs() const {
    return speedMs;
}

void VisualizationHandler::setSpeedMs(int v) {
    speedMs = v;
    if (speedMs < 0) speedMs = 0;
    if (speedMs > 5000) speedMs = 5000;
}

void VisualizationHandler::drawCurrentVisualization(Renderer& R, const sf::FloatRect& vizPane) {
    (void)R; (void)vizPane;
}
