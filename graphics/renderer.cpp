// renderer.cpp
#include "renderer.h"
#include <cmath>

Renderer::Renderer(sf::RenderWindow& w, sf::Font& f) : window(w), font(f) {}

void Renderer::roundedRect(const sf::FloatRect& r,
                            sf::Color outline, sf::Color fill,
                            float radius, float thick)
{
    sf::RectangleShape s({r.width, r.height});
    s.setPosition(r.left, r.top);
    s.setFillColor(fill);
    s.setOutlineColor(outline);
    s.setOutlineThickness(thick);
    s.setRadius(radius);
    window.draw(s);
}

void Renderer::checkbox(const sf::FloatRect& r,
                        const std::string& label,
                        bool checked, bool hovered)
{
    sf::RectangleShape box({18,18});
    box.setPosition(r.left, r.top);
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2);
    box.setOutlineColor(hovered?sf::Color(0,120,215):sf::Color::Black);
    window.draw(box);

    if (checked) {
        sf::RectangleShape tick({12,12});
        tick.setPosition(r.left+3, r.top+3);
        tick.setFillColor(sf::Color(0,180,0));
        window.draw(tick);
    }

    sf::Text t(label, font, 16);
    t.setFillColor(sf::Color::Black);
    t.setPosition(r.left+26, r.top-2);
    window.draw(t);
}

void Renderer::slider(const sf::FloatRect& track, float knobX)
{
    sf::RectangleShape line({track.width,4});
    line.setPosition(track.left, track.top+track.height/2-2);
    line.setFillColor(sf::Color(40,40,160));
    window.draw(line);

    sf::CircleShape knob(7);
    knob.setFillColor(sf::Color::Black);
    knob.setPosition(knobX-7, track.top+track.height/2-7);
    window.draw(knob);
}

void Renderer::textCenter(const sf::FloatRect& area,
                          const std::string& txt,
                          unsigned size,
                          sf::Color col)
{
    sf::Text t(txt, font, size);
    t.setFillColor(col);
    auto b = t.getLocalBounds();
    t.setOrigin(b.width/2.f, b.height/2.f);
    t.setPosition(area.left+area.width/2.f,
                  area.top +area.height/2.f);
    window.draw(t);
}
