// graphics/renderer.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>

class Renderer {
public:
    // Updated Theme Colors for a more modern feel
    static const sf::Color BACKGROUND_COLOR;        // Very dark grey, almost black
    static const sf::Color PANEL_BACKGROUND_COLOR;  // Dark grey
    static const sf::Color PRIMARY_UI_COLOR;        // Medium dark grey (for elements)
    static const sf::Color SECONDARY_UI_COLOR;      // Lighter grey (for slider tracks, etc.)
    static const sf::Color ACCENT_COLOR;            // Vibrant Blue
    static const sf::Color ACCENT_HOVER_COLOR;      // Lighter Blue for hover
    static const sf::Color TEXT_COLOR;              // Off-white
    static const sf::Color TEXT_MUTED_COLOR;        // Lighter grey for less important text
    static const sf::Color BORDER_COLOR;            // Subtle dark border
    static const sf::Color CHECKBOX_CHECK_COLOR;
    static const sf::Color SLIDER_KNOB_COLOR;
    static const sf::Color GRAPH_BORDER_COLOR;
    static const sf::Color AXIS_COLOR;
    static const sf::Color COMPLEXITY_TIME_COLOR;
    static const sf::Color COMPLEXITY_SPACE_COLOR;


    Renderer(sf::RenderWindow& w, sf::Font& f);

    void drawPanel(const sf::FloatRect& r, const std::string& title = "", unsigned titleSize = 18);
    void drawCheckbox(const sf::FloatRect& r, const std::string& label, bool checked, bool hovered, float checkboxSize, float labelOffset, int fontSize);
    // Overload for horizontal slider (used for DS bar)
    void drawHorizontalSlider(const sf::FloatRect& trackRect, float knobX, float knobWidth, float knobHeight = 12.f);
    // Original vertical/general slider (used for speed)
    void drawSlider(const sf::FloatRect& trackRect, float knobX, float knobRadius = 8.f); // For speed slider (circular knob)

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

    // Kept as sharp rect for now, true rounding is complex. Name implies intent.
    void drawStyledRect(const sf::FloatRect& r, sf::Color outline, sf::Color fill, float thick = 1.f);
    void drawAxes(const sf::FloatRect& graphPlotArea, unsigned max_n, double maxYValue);
};
