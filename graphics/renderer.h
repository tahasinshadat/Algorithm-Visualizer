// graphics/renderer.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>

// Class is now in the global namespace

class Renderer {
public:
    static const sf::Color BACKGROUND_COLOR;
    static const sf::Color PANEL_BACKGROUND_COLOR;
    static const sf::Color PRIMARY_UI_COLOR;
    static const sf::Color ACCENT_COLOR;
    static const sf::Color TEXT_COLOR;
    static const sf::Color BORDER_COLOR;
    static const sf::Color CHECKBOX_CHECK_COLOR;
    static const sf::Color SLIDER_KNOB_COLOR;
    static const sf::Color HOVER_COLOR;
    static const sf::Color COMPLEXITY_TIME_COLOR;
    static const sf::Color COMPLEXITY_SPACE_COLOR;
    static const sf::Color GRAPH_BORDER_COLOR;
    static const sf::Color AXIS_COLOR;

    Renderer(sf::RenderWindow& w, sf::Font& f);

    void drawPanel(const sf::FloatRect& r, const std::string& title = "");
    void drawCheckbox(const sf::FloatRect& r, const std::string& label, bool checked, bool hovered, float checkboxSize, float labelOffset, int fontSize);
    void drawSlider(const sf::FloatRect& trackRect, float knobX, float knobRadius = 8.f);
    void drawText(const std::string& txt, const sf::Vector2f& position, unsigned size = 18, sf::Color c = TEXT_COLOR, bool bold = false, bool italic = false);
    void drawTextCentered(const sf::FloatRect& area, const std::string& txt, unsigned size = 18, sf::Color c = TEXT_COLOR, float vOffset = 0.f);
    
    void drawFunctionGraph(const sf::FloatRect& bounds,
                           const std::string& bigO_notation,
                           sf::Color lineColor,
                           const std::string& title,
                           unsigned max_n_for_plot = 100);

    void drawArray(const sf::FloatRect& bounds, const std::vector<int>& arr,
                   const std::vector<sf::Color>& highlights = {});

private:
    sf::RenderWindow& window;
    sf::Font& font;

    void drawRoundedRect(const sf::FloatRect& r, sf::Color outline, sf::Color fill, float radius, float thick = 1.f);
    void drawAxes(const sf::FloatRect& graphPlotArea, unsigned max_n, double maxYValue);
};
