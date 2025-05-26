// graphics/visualization_handler.h
#pragma once
#include "../utilities/types.h" // Includes global enums
#include "renderer.h"
#include <vector>
#include <set>
#include <string>

// Class is now in the global namespace

class VisualizationHandler {
public:
    VisualizationHandler();

    void setDataStructure(DataStructure ds);
    void setAlgorithm(Algorithm alg);

    DataStructure getCurrentDataStructure() const;
    std::string getCurrentDataStructureDisplayName() const;
    Algorithm getCurrentAlgorithm() const;
    std::vector<Algorithm> getAvailableAlgorithms() const;

    int getSpeedMs() const;
    void setSpeedMs(int v);

    void drawCurrentVisualization(Renderer& R, const sf::FloatRect& vizPane);

private:
    DataStructure currentDS;
    Algorithm currentAlg;
    int speedMs;
};
