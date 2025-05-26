// gui.h
#pragma once
#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "visualization_handler.h"
#include "complexity_handler.h"

class GUI {
public:
    GUI(sf::RenderWindow& win, Renderer& r,
        VisualizationHandler& vh, ComplexityHandler& ch);

    void handleEvent(const sf::Event& e);
    void draw();

private:
    sf::RenderWindow&    win;
    Renderer&            R;
    VisualizationHandler& vh;
    ComplexityHandler&    comp;

    // geometry ----------------------------------------------------------
    const sf::FloatRect outer {10,10,780,630};
    const sf::FloatRect topBar{25,25,750,70};
    const sf::FloatRect vizPane{35,120,500,480};
    const sf::FloatRect algoPane{555,120,200,220};
    const sf::FloatRect sliderBox{555,360,200,60};
    const sf::FloatRect complexBox{555,430,200,170};

    // slider
    bool dragging = false;
    float knobX   = sliderBox.left+180;   // default speed

    // checkbox positions
    std::vector<sf::FloatRect> dsRect;
    std::vector<sf::FloatRect> algRect;
    void layoutCheckboxes();
};
