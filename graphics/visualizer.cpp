// graphics/visualizer.cpp
#include "visualizer.h"
#include <iostream>

// Using declarations for std types in .cpp file
using std::string;
using std::cout;
using std::cerr;
using std::endl;
// No custom namespace for av::VisualizerApp

VisualizerApp::VisualizerApp() { /* Defer initialization */ }
VisualizerApp::~VisualizerApp() { cleanup(); }

bool VisualizerApp::initialize() {
    if (!font.loadFromFile(FONT_PATH)) {
        cerr << "Error: Could not load font from " << FONT_PATH << endl;
        return false;
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Algorithm Visualizer", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    pRenderer = new Renderer(window, font);
    pVisHandler = new VisualizationHandler();
    pCompHandler = new ComplexityHandler();
    pGui = new GUI(window, *pRenderer, *pVisHandler, *pCompHandler, font);

    return true;
}

void VisualizerApp::cleanup() {
    delete pGui; pGui = nullptr;
    delete pCompHandler; pCompHandler = nullptr;
    delete pVisHandler; pVisHandler = nullptr;
    delete pRenderer; pRenderer = nullptr;
}

void VisualizerApp::run() {
    if (!initialize()) {
        return;
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (pGui) {
                pGui->handleEvent(event);
            }
        }
        window.clear(Renderer::BACKGROUND_COLOR); // Renderer is global
        if (pGui) {
            pGui->draw();
        }
        window.display();
    }
    cleanup();
}
