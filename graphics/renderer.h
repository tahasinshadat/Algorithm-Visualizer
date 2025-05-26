// renderer.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Renderer {
public:
    Renderer(sf::RenderWindow& w, sf::Font& f);

    void roundedRect(const sf::FloatRect& r,
                     sf::Color outline, sf::Color fill,
                     float thick = 2.f);                 // ← removed radius

    void checkbox(const sf::FloatRect& r,
                  const std::string& label,
                  bool checked, bool hovered);

    void slider(const sf::FloatRect& track, float knobX);
    void textCenter(const sf::FloatRect& area,
                    const std::string& txt,
                    unsigned size = 18,
                    sf::Color c     = sf::Color::Black);

private:
    sf::RenderWindow& window;
    sf::Font&         font;
};
