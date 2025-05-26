// graphics/renderer.cpp
#include "renderer.h"
#include "../utilities/types.h" // For evaluateSimpleComplexity
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

// Using declarations for std types in .cpp file
using std::string;
using std::vector;
using std::ostringstream;
using std::fixed;
using std::setprecision;
// No custom namespace for av::Renderer

const sf::Color Renderer::BACKGROUND_COLOR = sf::Color(30, 30, 30);
const sf::Color Renderer::PANEL_BACKGROUND_COLOR = sf::Color(45, 45, 45);
const sf::Color Renderer::PRIMARY_UI_COLOR = sf::Color(60, 60, 60);
const sf::Color Renderer::ACCENT_COLOR = sf::Color(0, 122, 255);
const sf::Color Renderer::TEXT_COLOR = sf::Color(220, 220, 220);
const sf::Color Renderer::BORDER_COLOR = sf::Color(20, 20, 20);
const sf::Color Renderer::CHECKBOX_CHECK_COLOR = Renderer::ACCENT_COLOR;
const sf::Color Renderer::SLIDER_KNOB_COLOR = Renderer::ACCENT_COLOR;
const sf::Color Renderer::HOVER_COLOR = sf::Color(80, 80, 80);
const sf::Color Renderer::COMPLEXITY_TIME_COLOR = sf::Color(70, 150, 255);
const sf::Color Renderer::COMPLEXITY_SPACE_COLOR = sf::Color(255, 99, 71);
const sf::Color Renderer::GRAPH_BORDER_COLOR = sf::Color(80, 80, 80);
const sf::Color Renderer::AXIS_COLOR = sf::Color(150, 150, 150);


Renderer::Renderer(sf::RenderWindow& w, sf::Font& f) : window(w), font(f) {}

void Renderer::drawRoundedRect(const sf::FloatRect& r, sf::Color outline, sf::Color fill, float radius, float thick) {
    (void)radius; 
    sf::RectangleShape s({r.width, r.height});
    s.setPosition(r.left, r.top);
    s.setFillColor(fill);
    s.setOutlineColor(outline);
    s.setOutlineThickness(thick);
    window.draw(s);
}

void Renderer::drawPanel(const sf::FloatRect& r, const string& title) {
    drawRoundedRect(r, BORDER_COLOR, PANEL_BACKGROUND_COLOR, 5.f, 1.f);
    if (!title.empty()) {
        drawText(title, {r.left + 10.f, r.top + 7.f}, 16, TEXT_COLOR);
    }
}

void Renderer::drawCheckbox(const sf::FloatRect& r, const string& label, bool checked, bool hovered, float checkboxSize, float labelOffset, int fontSize) {
    float boxY = r.top + (r.height - checkboxSize) / 2.f;
    sf::RectangleShape box({checkboxSize, checkboxSize});
    box.setPosition(r.left, boxY);
    box.setFillColor(PRIMARY_UI_COLOR);
    box.setOutlineThickness(1.f);
    box.setOutlineColor(hovered ? ACCENT_COLOR : BORDER_COLOR);
    window.draw(box);

    if (checked) {
        float checkPadding = checkboxSize / 4.f;
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

void Renderer::drawSlider(const sf::FloatRect& trackRect, float knobX, float knobRadius) {
    sf::RectangleShape line({trackRect.width, 4.f});
    line.setPosition(trackRect.left, trackRect.top + trackRect.height / 2.f - line.getSize().y / 2.f);
    line.setFillColor(PRIMARY_UI_COLOR);
    window.draw(line);
    sf::CircleShape knob(knobRadius);
    knob.setFillColor(SLIDER_KNOB_COLOR);
    knob.setOutlineColor(BORDER_COLOR);
    knob.setOutlineThickness(1.f);
    knob.setOrigin(knobRadius, knobRadius);
    knob.setPosition(knobX, trackRect.top + trackRect.height / 2.f);
    window.draw(knob);
}

void Renderer::drawText(const string& txt, const sf::Vector2f& position, unsigned size, sf::Color c, bool bold, bool italic) {
    sf::Text textObj(txt, font, size);
    textObj.setFillColor(c);
    textObj.setPosition(position);
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
    drawText("n", {graphPlotArea.left + graphPlotArea.width - 10, graphPlotArea.top + graphPlotArea.height - 15}, 10, AXIS_COLOR);
    drawText("0", {graphPlotArea.left - 5, graphPlotArea.top + graphPlotArea.height}, 10, AXIS_COLOR);
    
    ostringstream maxXLabel; maxXLabel << max_n;
    drawText(maxXLabel.str(), {graphPlotArea.left + graphPlotArea.width - 15, graphPlotArea.top + graphPlotArea.height + 2}, 10, AXIS_COLOR);

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = {graphPlotArea.left, graphPlotArea.top};
    yAxis[1].position = {graphPlotArea.left, graphPlotArea.top + graphPlotArea.height};
    yAxis[0].color = yAxis[1].color = AXIS_COLOR;
    window.draw(yAxis);
    drawText("Ops", {graphPlotArea.left + 5, graphPlotArea.top - 5}, 10, AXIS_COLOR);

    ostringstream maxYLabel;
    if (maxYValue < 1000 && maxYValue > -1000 && maxYValue !=0) maxYLabel << fixed << setprecision(1) << maxYValue;
    else if (maxYValue != 0) maxYLabel << fixed << setprecision(0) << maxYValue;
    else maxYLabel << "0";
    // Adjust Y-axis label position to avoid overlap if it's long
    sf::Text tempMaxYText(maxYLabel.str(), font, 10);
    drawText(maxYLabel.str(), {graphPlotArea.left - tempMaxYText.getLocalBounds().width - 7, graphPlotArea.top - 5}, 10, AXIS_COLOR);
}

void Renderer::drawFunctionGraph(const sf::FloatRect& bounds,
                                 const string& bigO_notation,
                                 sf::Color lineColor,
                                 const string& title,
                                 unsigned max_n_for_plot) {
    sf::RectangleShape graphBg(sf::Vector2f(bounds.width, bounds.height));
    graphBg.setPosition(bounds.left, bounds.top);
    graphBg.setFillColor(PRIMARY_UI_COLOR);
    graphBg.setOutlineColor(GRAPH_BORDER_COLOR);
    graphBg.setOutlineThickness(1.f);
    window.draw(graphBg);

    drawTextCentered(sf::FloatRect(bounds.left, bounds.top, bounds.width, 20), title + ": " + bigO_notation, 12, TEXT_COLOR);

    float padding = 30.f; // Increased padding for labels and axis values
    sf::FloatRect plotArea(bounds.left + padding, bounds.top + padding,
                           bounds.width - 1.5f * padding, bounds.height - 1.5f * padding);
    
    if (plotArea.width <=10 || plotArea.height <=10) { // Check for minimal drawable area
        drawTextCentered(plotArea, "Area too small", 10, TEXT_COLOR);
        return;
    }

    vector<sf::Vector2f> points;
    double maxYValue = 0.0;
    bool plottable = true;

    for (unsigned n_idx = 0; n_idx <= max_n_for_plot; ++n_idx) { // Start from n=0 or 1 for plot points
        double n_val = static_cast<double>(n_idx);
        if (n_idx == 0 && (bigO_notation == "O(log n)" || bigO_notation == "O(n log n)")) { // Avoid log(0) by starting plot at n=1 for log functions
             n_val = 1.0; // or skip n=0 for log
        }
        if (n_idx == 0 && n_val == 0) n_val = 1e-9; // if we must plot n=0 for non-log

        double val = evaluateSimpleComplexity(bigO_notation, n_val);
        if (std::isnan(val)) {
            plottable = false; break;
        }
        if (std::isinf(val) || val > 1e9) val = 1e9;
        
        // Store actual n_val used for x-coordinate
        points.push_back(sf::Vector2f(static_cast<float>(n_val == 1e-9 && n_idx == 0 ? 0 : n_val), static_cast<float>(val)));
        if (val > maxYValue && !std::isinf(val)) maxYValue = val;
    }

    if (maxYValue == 0 && !points.empty() && plottable) {
         maxYValue = 1.0; 
         for(const auto& p : points) if(p.y > maxYValue) maxYValue = p.y;
         if(maxYValue < 1.0 && maxYValue != 0.0) maxYValue = 1.0; // If all points are < 1 but not all 0
         else if(maxYValue == 0.0) maxYValue = 1.0; // All points are 0, e.g. O(0) effectively for n=0
    }

    if (!plottable || points.empty()) {
        drawTextCentered(plotArea, "Graph N/A for\n" + bigO_notation, 12, TEXT_COLOR);
        return;
    }
    
    drawAxes(plotArea, max_n_for_plot, maxYValue);

    sf::VertexArray lineStrip(sf::LineStrip);
    for (const auto& p : points) {
        float x_ratio = (p.x / static_cast<float>(max_n_for_plot));
        float screenX = plotArea.left + x_ratio * plotArea.width;
        
        float y_ratio = (maxYValue > 1e-9) ? (p.y / static_cast<float>(maxYValue)) : 0.0f; // Avoid division by zero if maxYValue is effectively zero
        float screenY = plotArea.top + plotArea.height - (y_ratio * plotArea.height);
        
        screenX = std::max(plotArea.left, std::min(screenX, plotArea.left + plotArea.width));
        screenY = std::max(plotArea.top, std::min(screenY, plotArea.top + plotArea.height));
        lineStrip.append(sf::Vertex(sf::Vector2f(screenX, screenY), lineColor));
    }

    if (lineStrip.getVertexCount() > 1) { // Need at least 2 points to draw a line
        window.draw(lineStrip);
    } else if (lineStrip.getVertexCount() == 1) { // Draw a single point if only one
        sf::CircleShape point(2.f);
        point.setFillColor(lineColor);
        point.setOrigin(1.f, 1.f);
        point.setPosition(lineStrip[0].position);
        window.draw(point);
    }
}

void Renderer::drawArray(const sf::FloatRect& bounds, const vector<int>& arr, const vector<sf::Color>& highlights) {
    (void)bounds; (void)arr; (void)highlights;
}
