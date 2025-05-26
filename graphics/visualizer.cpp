#include "visualizer.h"

void VisualizerApp::run() {
    sf::ContextSettings s; s.antialiasingLevel=8;
    sf::RenderWindow win(sf::VideoMode(W,H),"Algorithm Visualizer",
                         sf::Style::Default, s);
    win.setFramerateLimit(60);

    sf::Font font; 
    font.loadFromFile("assets/arial.ttf");
    Renderer R(win);

    VisualizationHandler vh;
    ComplexityHandler    ch;
    GUI gui(win,R,vh,ch);

    while(win.isOpen()){
        sf::Event e;
        while(win.pollEvent(e)){
            if(e.type==sf::Event::Closed) win.close();
            gui.handleEvent(e);
        }
        win.clear(sf::Color::White);
        gui.draw();
        win.display();
    }
}
