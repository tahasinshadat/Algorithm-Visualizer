// graphics/renderer.cpp
#include "renderer.h"
#include "../utilities/types.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using std::string;
using std::vector;
using std::ostringstream;
using std::fixed;
using std::setprecision;

// Updated Theme Colors
const sf::Color Renderer::BACKGROUND_COLOR = sf::Color(28, 28, 32);        // Very dark grey
const sf::Color Renderer::PANEL_BACKGROUND_COLOR = sf::Color(42, 42, 48);  // Dark grey
const sf::Color Renderer::PRIMARY_UI_COLOR = sf::Color(55, 55, 62);        // Medium dark grey
const sf::Color Renderer::SECONDARY_UI_COLOR = sf::Color(70, 70, 78);      // Lighter grey for tracks
const sf::Color Renderer::ACCENT_COLOR = sf::Color(0, 122, 255);            // Vibrant Blue
const sf::Color Renderer::ACCENT_HOVER_COLOR = sf::Color(50, 152, 255);     // Lighter Blue
const sf::Color Renderer::TEXT_COLOR = sf::Color(230, 230, 235);           // Off-white
const sf::Color Renderer::TEXT_MUTED_COLOR = sf::Color(160, 160, 170);      // Muted grey text
const sf::Color Renderer::BORDER_COLOR = sf::Color(35, 35, 40);            // Subtle dark border
const sf::Color Renderer::CHECKBOX_CHECK_COLOR = Renderer::ACCENT_COLOR;
const sf::Color Renderer::SLIDER_KNOB_COLOR = Renderer::ACCENT_COLOR;
const sf::Color Renderer::GRAPH_BORDER_COLOR = sf::Color(60, 60, 68);
const sf::Color Renderer::AXIS_COLOR = sf::Color(130, 130, 140);
const sf::Color Renderer::COMPLEXITY_TIME_COLOR = sf::Color(70, 150, 255);
const sf::Color Renderer::COMPLEXITY_SPACE_COLOR = sf::Color(255, 99, 71);


Renderer::Renderer(sf::RenderWindow& w, sf::Font& f) : window(w), font(f) {}

void Renderer::drawStyledRect(const sf::FloatRect& r, sf::Color outline, sf::Color fill, float thick) {
    sf::RectangleShape s({r.width, r.height});
    s.setPosition(r.left, r.top);
    s.setFillColor(fill);
    s.setOutlineColor(outline);
    s.setOutlineThickness(thick);
    window.draw(s);
}

void Renderer::drawPanel(const sf::FloatRect& r, const string& title, unsigned titleSize) {
    drawStyledRect(r, BORDER_COLOR, PANEL_BACKGROUND_COLOR, 1.f);
    if (!title.empty()) {
        // Make panel titles a bit more prominent
        drawText(title, {r.left + 12.f, r.top + 8.f}, titleSize, TEXT_COLOR, true);
    }
}

void Renderer::drawCheckbox(const sf::FloatRect& r, const string& label, bool checked, bool hovered, float checkboxSize, float labelOffset, int fontSize) {
    float boxY = r.top + (r.height - checkboxSize) / 2.f;
    sf::RectangleShape box({checkboxSize, checkboxSize});
    box.setPosition(r.left, boxY);
    box.setFillColor(hovered && !checked ? SECONDARY_UI_COLOR : PRIMARY_UI_COLOR); // Subtle hover fill
    box.setOutlineThickness(1.f);
    box.setOutlineColor(hovered || checked ? ACCENT_COLOR : BORDER_COLOR); // Accent outline on hover/check
    window.draw(box);

    if (checked) {
        float checkPadding = checkboxSize / 3.5f;
        sf::RectangleShape tick({checkboxSize - 2 * checkPadding, checkboxSize - 2 * checkPadding});
        tick.setPosition(r.left + checkPadding, box.getPosition().y + checkPadding);
        tick.setFillColor(CHECKBOX_CHECK_COLOR);
        window.draw(tick);
    }
    sf::Text t(label, font, static_cast<unsigned int>(fontSize));
    t.setFillColor(TEXT_COLOR);
    sf::FloatRect textBounds = t.getLocalBounds();
    t.setPosition(r.left + checkboxSize + labelOffset, r.top + (r.height - textBounds.height - textBounds.top) / 2.f );
    window.draw(t);
}

// For speed slider (circular knob)
void Renderer::drawSlider(const sf::FloatRect& trackRect, float knobX, float knobRadius) {
    sf::RectangleShape line({trackRect.width, 6.f}); // Thicker track
    line.setPosition(trackRect.left, trackRect.top + trackRect.height / 2.f - line.getSize().y / 2.f);
    line.setFillColor(SECONDARY_UI_COLOR);
    // line.setRadius(3.f); // If SFML supports rounded rects for track
    window.draw(line);

    sf::CircleShape knob(knobRadius);
    knob.setFillColor(SLIDER_KNOB_COLOR);
    knob.setOutlineColor(BACKGROUND_COLOR); // Contrast outline
    knob.setOutlineThickness(2.f);
    knob.setOrigin(knobRadius, knobRadius);
    knob.setPosition(knobX, trackRect.top + trackRect.height / 2.f);
    window.draw(knob);
}

// For DS bar (rectangular knob/thumb)
void Renderer::drawHorizontalSlider(const sf::FloatRect& trackRect, float knobX, float knobWidth, float knobHeight) {
    sf::RectangleShape trackShape({trackRect.width, trackRect.height});
    trackShape.setPosition(trackRect.left, trackRect.top);
    trackShape.setFillColor(SECONDARY_UI_COLOR); // Track color
    // trackShape.setRadius(trackRect.height / 2.f); // Rounded track
    window.draw(trackShape);

    sf::RectangleShape knobShape({knobWidth, knobHeight});
    knobShape.setPosition(knobX, trackRect.top + (trackRect.height - knobHeight) / 2.f); // Center knob vertically
    knobShape.setFillColor(ACCENT_COLOR); // Knob color
    // knobShape.setRadius(knobHeight / 2.f); // Rounded knob
    window.draw(knobShape);
}


void Renderer::drawText(const string& txt, const sf::Vector2f& position, unsigned size, sf::Color c, bool bold, bool italic) {
    sf::Text textObj(txt, font, size);
    textObj.setFillColor(c);
    textObj.setPosition(position.x, position.y - size/10.f); // Minor baseline adjustment
    sf::Uint32 style = sf::Text::Regular;
    if (bold) style |= sf::Text::Bold;
    if (italic) style |= sf::Text::Italic;
    textObj.setStyle(style);
    window.draw(textObj);
}

void Renderer::drawTextCentered(const sf::FloatRect& area, const string& txt, unsigned size, sf::Color c, float vOffset) {
    sf::Text textObj(txt, font, size);
    textObj.setFillColor(c);
    sf::FloatRect textBounds = textObj.getLocalBounds();
    // Proper centering: consider textBounds.left and textBounds.top for origin
    textObj.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    textObj.setPosition(area.left + area.width / 2.0f, area.top + area.height / 2.0f + vOffset);
    window.draw(textObj);
}

void Renderer::drawAxes(const sf::FloatRect& graphPlotArea, unsigned max_n, double maxYValue) {
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = {graphPlotArea.left, graphPlotArea.top + graphPlotArea.height};
    xAxis[1].position = {graphPlotArea.left + graphPlotArea.width, graphPlotArea.top + graphPlotArea.height};
    xAxis[0].color = xAxis[1].color = AXIS_COLOR;
    window.draw(xAxis);
    drawText("n", {graphPlotArea.left + graphPlotArea.width - 12, graphPlotArea.top + graphPlotArea.height - 18}, 11, TEXT_MUTED_COLOR);
    drawText("0", {graphPlotArea.left - 8, graphPlotArea.top + graphPlotArea.height - 5}, 11, TEXT_MUTED_COLOR);
    
    ostringstream maxXLabel; maxXLabel << max_n;
    sf::Text tempMaxXText(maxXLabel.str(), font, 11);
    drawText(maxXLabel.str(), {graphPlotArea.left + graphPlotArea.width - tempMaxXText.getLocalBounds().width - 2, graphPlotArea.top + graphPlotArea.height + 3}, 11, TEXT_MUTED_COLOR);

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = {graphPlotArea.left, graphPlotArea.top};
    yAxis[1].position = {graphPlotArea.left, graphPlotArea.top + graphPlotArea.height};
    yAxis[0].color = yAxis[1].color = AXIS_COLOR;
    window.draw(yAxis);
    drawText("Value", {graphPlotArea.left + 5, graphPlotArea.top - 1}, 11, TEXT_MUTED_COLOR);

    ostringstream maxYLabel;
    if (maxYValue < 1000 && maxYValue > -1000 && maxYValue !=0) maxYLabel << fixed << setprecision(1) << maxYValue;
    else if (maxYValue != 0) maxYLabel << fixed << setprecision(0) << maxYValue;
    else maxYLabel << "0";
    sf::Text tempMaxYText(maxYLabel.str(), font, 11);
    drawText(maxYLabel.str(), {graphPlotArea.left - tempMaxYText.getLocalBounds().width - 10, graphPlotArea.top - 5}, 11, TEXT_MUTED_COLOR);
}

void Renderer::drawFunctionGraph(const sf::FloatRect& bounds,
                                 const string& bigO_notation,
                                 sf::Color lineColor,
                                 const string& title,
                                 unsigned max_n_for_plot) {
    sf::RectangleShape graphBg(sf::Vector2f(bounds.width, bounds.height));
    graphBg.setPosition(bounds.left, bounds.top);
    graphBg.setFillColor(PANEL_BACKGROUND_COLOR); // Use panel bg for graph area for contrast
    graphBg.setOutlineColor(GRAPH_BORDER_COLOR);
    graphBg.setOutlineThickness(1.f);
    window.draw(graphBg);

    drawText(title + ": " + bigO_notation, {bounds.left + 5, bounds.top + 2}, 13, TEXT_COLOR, true);

    float titleAreaHeight = 25.f;
    float axisPadding = 30.f; // Increased padding for labels
    sf::FloatRect plotArea(bounds.left + axisPadding, bounds.top + titleAreaHeight + 5, // 5 for spacing below title
                           bounds.width - axisPadding - 15.f, // 15 for right padding
                           bounds.height - titleAreaHeight - axisPadding - 10.f); // 10 for bottom padding
    
    if (plotArea.width <=10 || plotArea.height <=10) {
        drawTextCentered(bounds, "Plot area too small", 12, TEXT_MUTED_COLOR);
        return;
    }

    vector<sf::Vector2f> points;
    double maxYValue = 0.0;
    bool plottable = true;

    for (unsigned n_idx = 0; n_idx <= max_n_for_plot; ++n_idx) {
        double n_val = static_cast<double>(n_idx);
        if (n_idx == 0 && (bigO_notation.find("log n") != string::npos)) n_val = 1.0;
        if (n_idx == 0 && n_val == 0) n_val = 1e-9;

        double val = evaluateSimpleComplexity(bigO_notation, n_val);
        if (std::isnan(val)) { plottable = false; break; }
        if (std::isinf(val) || val > 1e9) val = 1e9;
        
        points.push_back(sf::Vector2f(static_cast<float>(n_idx), static_cast<float>(val))); // Use n_idx for x-axis mapping
        if (val > maxYValue && !std::isinf(val)) maxYValue = val;
    }

    if (maxYValue == 0 && !points.empty() && plottable) {
         maxYValue = 1.0; 
         for(const auto& p : points) if(p.y > maxYValue) maxYValue = p.y;
         if(maxYValue < 1.0 && maxYValue != 0.0) maxYValue = 1.0;
         else if(maxYValue == 0.0) maxYValue = 1.0;
    }

    if (!plottable || points.empty()) {
        drawTextCentered(plotArea, "Graph N/A for\n" + bigO_notation, 13, TEXT_MUTED_COLOR);
        return;
    }
    
    drawAxes(plotArea, max_n_for_plot, maxYValue);

    sf::VertexArray lineStrip(sf::LineStrip);
    for (const auto& p : points) { // p.x is n_idx (0 to max_n_for_plot)
        float x_ratio = (max_n_for_plot > 0) ? (p.x / static_cast<float>(max_n_for_plot)) : 0.f;
        float screenX = plotArea.left + x_ratio * plotArea.width;
        
        float y_ratio = (maxYValue > 1e-9) ? (p.y / static_cast<float>(maxYValue)) : 0.0f;
        float screenY = plotArea.top + plotArea.height - (y_ratio * plotArea.height);
        
        screenX = std::max(plotArea.left, std::min(screenX, plotArea.left + plotArea.width));
        screenY = std::max(plotArea.top, std::min(screenY, plotArea.top + plotArea.height));
        lineStrip.append(sf::Vertex(sf::Vector2f(screenX, screenY), lineColor));
    }

    if (lineStrip.getVertexCount() > 1) window.draw(lineStrip);
    else if (lineStrip.getVertexCount() == 1) {
        sf::CircleShape point(2.f); point.setFillColor(lineColor);
        point.setOrigin(1.f, 1.f); point.setPosition(lineStrip[0].position);
        window.draw(point);
    }
}

void Renderer::drawArray(const sf::FloatRect& bounds, const vector<int>& arr, const vector<sf::Color>& highlights) {
    (void)bounds; (void)arr; (void)highlights;
}
