// graphics/visualizer.h
#pragma once
#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "gui.h"
#include "visualization_handler.h"
#include "complexity_handler.h"
#include <string>

// Class is now in the global namespace

class VisualizerApp {
public:
    VisualizerApp();
    ~VisualizerApp();
    void run();

private:
    const unsigned int WINDOW_WIDTH = 1250; 
    const unsigned int WINDOW_HEIGHT = 850;
    const std::string FONT_PATH = "assets/ArialCE.ttf";

    sf::RenderWindow window;
    sf::Font font;

    Renderer* pRenderer = nullptr;
    VisualizationHandler* pVisHandler = nullptr;
    ComplexityHandler* pCompHandler = nullptr;
    GUI* pGui = nullptr;

    bool initialize();
    void cleanup();
};
