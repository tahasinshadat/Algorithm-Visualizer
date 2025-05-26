#pragma once
#include "types.h"

class VisualizationHandler {
public:
    VisualizationHandler() : currentDS(DataStructure::Graph) {}

    void setDataStructure(DataStructure ds) { currentDS = ds; }
    void setAlgorithm(Algorithm alg)        { currentAlg = alg; }

    DataStructure getDataStructure() const { return currentDS; }
    Algorithm     getAlgorithm()     const { return currentAlg; }

    int  getSpeedMs()          const { return speedMs; }
    void setSpeedMs(int v)           { speedMs = v;   }

private:
    DataStructure currentDS;
    Algorithm     currentAlg = Algorithm::None;
    int           speedMs    = 300;
};
