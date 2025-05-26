// visualizer.h
#pragma once
#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "gui.h"
#include "visualization_handler.h"
#include "complexity_handler.h"

class VisualizerApp {
public:
    void run();          // opens window & loop
private:
    const unsigned W=800, H=650;
};
