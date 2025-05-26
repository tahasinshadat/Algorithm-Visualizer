// graphics/gui.cpp
#include "gui.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <set>

using std::string;
using std::vector;
using std::set;
using std::max;
using std::min;

GUI::GUI(sf::RenderWindow& win_, Renderer& r_,
         VisualizationHandler& vh_, ComplexityHandler& ch_, sf::Font& font_)
: window(win_), renderer(r_), visHandler(vh_), compHandler(ch_), appFont(font_) {
    initializeLayout(window.getSize().x, window.getSize().y);
    populateDSCheckboxes();
    layoutDSCheckboxes(); // Initial layout based on offset 0
    updateDSSliderKnobPositionFromScrollOffset(); // Init DS slider knob

    populateAlgorithmCheckboxes();
    updateSliderKnobPositionFromSpeed();
}

void GUI::initializeLayout(unsigned int windowWidth, unsigned int windowHeight) {
    float rightPaneWidth = 300.f; // Wider for better graph display
    float dsBarContentHeight = 40.f; // Space for DS items
    float dsSliderAreaHeight = 20.f; // Space for the DS slider
    float dsBarHeight = TITLE_AREA_HEIGHT + dsBarContentHeight + dsSliderAreaHeight + PADDING/2.f;


    topBarRect = {PADDING, PADDING, windowWidth - 2 * PADDING, dsBarHeight};
    // Visible width for DS items, slider will be below them
    dsListVisibleWidth = topBarRect.width - 2 * PADDING; 

    visualizationPaneRect = {PADDING, topBarRect.top + topBarRect.height + PADDING,
                             windowWidth - rightPaneWidth - 3 * PADDING,
                             windowHeight - (topBarRect.top + topBarRect.height + PADDING) - PADDING};

    float rightPaneTotalHeight = windowHeight - (topBarRect.top + topBarRect.height + PADDING) - PADDING;
    float algoPaneHeight = rightPaneTotalHeight * 0.32f;
    float speedSliderHeight = 75.f;
    float complexityPaneHeight = rightPaneTotalHeight - algoPaneHeight - speedSliderHeight - 2 * PADDING;

    algorithmPaneRect = {visualizationPaneRect.left + visualizationPaneRect.width + PADDING,
                         topBarRect.top + topBarRect.height + PADDING,
                         rightPaneWidth,
                         algoPaneHeight};

    speedSliderPaneRect = {algorithmPaneRect.left,
                           algorithmPaneRect.top + algorithmPaneRect.height + PADDING,
                           rightPaneWidth,
                           speedSliderHeight};

    complexityPaneRect = {algorithmPaneRect.left,
                          speedSliderPaneRect.top + speedSliderPaneRect.height + PADDING,
                          rightPaneWidth,
                          complexityPaneHeight};
    
    float graphTitleMargin = 5.f;
    float complexityTextHeight = 45.f;
    float availableGraphHeight = (complexityPaneRect.height - TITLE_AREA_HEIGHT - complexityTextHeight - 2 * graphTitleMargin - 2 * PADDING ) / 2.f;
    
    timeComplexityGraphRect = {complexityPaneRect.left + PADDING,
                               complexityPaneRect.top + TITLE_AREA_HEIGHT + PADDING,
                               complexityPaneRect.width - 2 * PADDING,
                               max(60.f, availableGraphHeight)};

    spaceComplexityGraphRect = {complexityPaneRect.left + PADDING,
                                timeComplexityGraphRect.top + timeComplexityGraphRect.height + PADDING + graphTitleMargin,
                                complexityPaneRect.width - 2 * PADDING,
                                max(60.f, availableGraphHeight)};

    algListVisibleHeight = algorithmPaneRect.height - TITLE_AREA_HEIGHT - PADDING;
    algScrollbarBack.setSize({12.f, algListVisibleHeight}); // Slightly wider scrollbar
    algScrollbarBack.setFillColor(Renderer::SECONDARY_UI_COLOR);
    algScrollbarBack.setPosition(algorithmPaneRect.left + algorithmPaneRect.width - PADDING - 12.f,
                                 algorithmPaneRect.top + TITLE_AREA_HEIGHT);
    
    // DS Slider Track layout (below DS items, within topBarRect)
    dsSliderTrackRect = {topBarRect.left + PADDING,
                         topBarRect.top + TITLE_AREA_HEIGHT + dsBarContentHeight + PADDING / 2.f,
                         topBarRect.width - 2 * PADDING,
                         DS_SLIDER_TRACK_HEIGHT};
}

void GUI::populateDSCheckboxes() {
    dsCheckboxItems.clear();
    dsListTotalWidth = 0;
    set<string> displayedNames;

    for (int i = 1; i < static_cast<int>(DataStructure::SegmentTree) + 1; ++i) {
        DataStructure currentEnum = static_cast<DataStructure>(i);
        string name = DATA_STRUCTURE_NAMES[i];
        if (currentEnum == DataStructure::Vector) continue;
        if (displayedNames.find(name) == displayedNames.end() || name == ARRAY_VECTOR_DISPLAY_NAME) {
             if (name == ARRAY_VECTOR_DISPLAY_NAME && displayedNames.count(name)) continue;
            displayedNames.insert(name);
            sf::Text tempText(name, appFont, CHECKBOX_FONT_SIZE);
            float textWidth = tempText.getLocalBounds().width;
            float itemWidth = CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X + textWidth + PADDING * 1.8f; // Increased item padding
            itemWidth = max(itemWidth, DS_ITEM_MIN_WIDTH);
            dsCheckboxItems.push_back({
                {0,0,0,0}, name, currentEnum, Algorithm::None, true, true
            });
            dsListTotalWidth += itemWidth + DS_ITEM_SPACING;
        }
    }
    if (!dsCheckboxItems.empty()) dsListTotalWidth -= DS_ITEM_SPACING;
}

void GUI::layoutDSCheckboxes() {
    // Y position for DS checkboxes, below the "Data Structure" title
    float yPos = topBarRect.top + TITLE_AREA_HEIGHT - 5.f;
    // Starting X, considering the scroll offset
    float currentX = topBarRect.left + PADDING - dsListScrollOffset;

    for (CheckboxUIData& cb : dsCheckboxItems) {
        sf::Text tempText(cb.label, appFont, CHECKBOX_FONT_SIZE);
        float textWidth = tempText.getLocalBounds().width;
        float itemWidth = CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X + textWidth + PADDING * 1.8f;
        itemWidth = max(itemWidth, DS_ITEM_MIN_WIDTH);

        cb.rect = {currentX, yPos, itemWidth, CHECKBOX_ITEM_HEIGHT};
        // Visibility check based on whether it's within the designated visible area for DS items
        cb.isVisible = (cb.rect.left + cb.rect.width > topBarRect.left + PADDING &&
                        cb.rect.left < topBarRect.left + PADDING + dsListVisibleWidth);
        currentX += itemWidth + DS_ITEM_SPACING;
    }
}

void GUI::populateAlgorithmCheckboxes() {
    algCheckboxItems.clear();
    algListTotalHeight = 0;
    vector<Algorithm> availableAlgs = visHandler.getAvailableAlgorithms();
    if (!availableAlgs.empty()) {
        for (Algorithm algEnum : availableAlgs) {
            if (static_cast<size_t>(algEnum) < ALGORITHM_NAMES.size()) {
                algCheckboxItems.push_back({
                    {0,0,0,0}, ALGORITHM_NAMES[static_cast<int>(algEnum)],
                    DataStructure::None, algEnum, false, true
                });
                algListTotalHeight += CHECKBOX_ITEM_HEIGHT;
            }
        }
    }
    algListScrollOffset = 0;
    layoutAlgorithmCheckboxes();
}

void GUI::layoutAlgorithmCheckboxes() {
    float currentY_Alg = algorithmPaneRect.top + TITLE_AREA_HEIGHT - algListScrollOffset;
    float xPosAlg = algorithmPaneRect.left + PADDING;
    float algItemWidth = algorithmPaneRect.width - 2 * PADDING - (algListTotalHeight > algListVisibleHeight ? 18.f : 0.f); // Wider scrollbar allowance

    for (CheckboxUIData& cbInfo : algCheckboxItems) {
        cbInfo.rect = {xPosAlg, currentY_Alg, algItemWidth, CHECKBOX_ITEM_HEIGHT};
        currentY_Alg += CHECKBOX_ITEM_HEIGHT;
    }

    if (algListTotalHeight > algListVisibleHeight) {
        float thumbHeightRatio = algListVisibleHeight / algListTotalHeight;
        float thumbHeight = max(25.f, algListVisibleHeight * thumbHeightRatio); // Taller thumb
        algScrollbarThumb.setSize({12.f, thumbHeight});
        algScrollbarThumb.setFillColor(Renderer::PRIMARY_UI_COLOR);
        float scrollableDist = algListTotalHeight - algListVisibleHeight;
        float scrollOffsetRatio = (scrollableDist > 0.f) ? (algListScrollOffset / scrollableDist) : 0.f;
        float thumbMaxYTravel = algListVisibleHeight - thumbHeight;
        algScrollbarThumb.setPosition(algScrollbarBack.getPosition().x,
                                      algScrollbarBack.getPosition().y + scrollOffsetRatio * thumbMaxYTravel);
    }
}

void GUI::updateDSSliderKnobPositionFromScrollOffset() {
    if (dsListTotalWidth <= dsListVisibleWidth) { // No scroll needed
        dsSliderKnobX = dsSliderTrackRect.left;
        dsSliderKnobWidth = dsSliderTrackRect.width; // Knob takes full width
        return;
    }
    // Calculate knob width based on visible ratio
    dsSliderKnobWidth = max(30.f, dsSliderTrackRect.width * (dsListVisibleWidth / dsListTotalWidth));
    float scrollableTrackWidth = dsSliderTrackRect.width - dsSliderKnobWidth;
    float scrollRatio = dsListScrollOffset / (dsListTotalWidth - dsListVisibleWidth);
    dsSliderKnobX = dsSliderTrackRect.left + scrollRatio * scrollableTrackWidth;
    dsSliderKnobX = std::clamp(dsSliderKnobX, dsSliderTrackRect.left, dsSliderTrackRect.left + dsSliderTrackRect.width - dsSliderKnobWidth);
}

void GUI::updateDSScrollOffsetFromSliderKnobPosition() {
    if (dsListTotalWidth <= dsListVisibleWidth) {
        dsListScrollOffset = 0;
        return;
    }
    float scrollableTrackWidth = dsSliderTrackRect.width - dsSliderKnobWidth;
    if (scrollableTrackWidth <= 0) { // Knob is full width, no scroll
        dsListScrollOffset = 0;
        return;
    }
    float knobRatio = (dsSliderKnobX - dsSliderTrackRect.left) / scrollableTrackWidth;
    dsListScrollOffset = knobRatio * (dsListTotalWidth - dsListVisibleWidth);
    dsListScrollOffset = std::clamp(dsListScrollOffset, 0.f, dsListTotalWidth - dsListVisibleWidth);
    layoutDSCheckboxes(); // Re-layout DS items based on new scroll offset
}


void GUI::updateSliderKnobPositionFromSpeed() {
    float minX = speedSliderPaneRect.left + PADDING + 10.f;
    float maxX = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
    float trackWidth = maxX - minX;
    if (trackWidth <= 0) return;
    float maxSpeed = 2000.f;
    float currentSpeed = static_cast<float>(visHandler.getSpeedMs());
    speedSliderKnobX = minX + (currentSpeed / maxSpeed) * trackWidth;
    std::cout << speedSliderKnobX << std::endl;
    speedSliderKnobX = std::clamp(speedSliderKnobX, minX, maxX);
}
void GUI::updateSpeedFromSliderKnobPosition() {
    float minX = speedSliderPaneRect.left + PADDING + 10.f;
    float maxX = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
    float trackWidth = maxX - minX;
    if (trackWidth <= 0) return;
    float ratio = (dsSliderKnobX - minX) / trackWidth;
    float maxSpeed = 2000.f;
    int newSpeed = static_cast<int>(std::round(ratio * maxSpeed));
    newSpeed = std::clamp(newSpeed, 0, static_cast<int>(maxSpeed));
    visHandler.setSpeedMs(newSpeed);
}

void GUI::handleEvent(const sf::Event& e) {
    mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    if (e.type == sf::Event::MouseButtonPressed) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            // DS Checkbox Clicks
            for (const auto& cb : dsCheckboxItems) {
                if (cb.isVisible && cb.rect.contains(mousePos)) {
                    visHandler.setDataStructure(cb.dsEnum);
                    populateAlgorithmCheckboxes(); // This calls layoutAlgorithmCheckboxes
                    return;
                }
            }
            // DS Slider Drag Start
            sf::FloatRect dsSliderKnobRect = {dsSliderKnobX, dsSliderTrackRect.top, dsSliderKnobWidth, dsSliderTrackRect.height * DS_SLIDER_KNOB_HEIGHT_FACTOR};
            if (dsSliderTrackRect.contains(mousePos) || dsSliderKnobRect.contains(mousePos)) { // Click on track or knob
                 if (dsListTotalWidth > dsListVisibleWidth) { // Only allow drag if scrollable
                    isDraggingDSSlider = true;
                    // Snap knob to click X if clicking on track, adjust offset
                    if (dsSliderTrackRect.width - dsSliderKnobWidth > 0) { // Ensure there's space to move knob
                        float clickRatio = (mousePos.x - dsSliderTrackRect.left - dsSliderKnobWidth / 2.f) / (dsSliderTrackRect.width - dsSliderKnobWidth);
                        clickRatio = std::clamp(clickRatio, 0.f, 1.f);
                        dsListScrollOffset = clickRatio * (dsListTotalWidth - dsListVisibleWidth);
                        updateDSSliderKnobPositionFromScrollOffset();
                        layoutDSCheckboxes();
                    }
                    return;
                }
            }
            // Algorithm Checkbox Clicks (same as before)
            for (const auto& cb : algCheckboxItems) { /* ... */ 
                if (cb.rect.top + cb.rect.height > algorithmPaneRect.top + TITLE_AREA_HEIGHT &&
                    cb.rect.top < algorithmPaneRect.top + TITLE_AREA_HEIGHT + algListVisibleHeight &&
                    cb.rect.contains(mousePos)) {
                    visHandler.setAlgorithm(cb.algEnum); return;
                }
            }
            // Speed Slider Drag Start (same as before)
            sf::FloatRect speedSliderTrackClickArea( speedSliderPaneRect.left + PADDING, speedSliderPaneRect.top + TITLE_AREA_HEIGHT,
                speedSliderPaneRect.width - 2 * PADDING, speedSliderPaneRect.height - TITLE_AREA_HEIGHT - PADDING);
            if (speedSliderTrackClickArea.contains(mousePos)) { /* ... */ 
                 isDraggingSpeedSlider = true;
                 float minXTrack = speedSliderPaneRect.left + PADDING + 10.f;
                 float maxXTrack = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
                 speedSliderKnobX = std::clamp(mousePos.x, minXTrack, maxXTrack);
                 updateSpeedFromSliderKnobPosition(); return;
            }
            // Algorithm Scrollbar Drag Start (same as before)
            if (algListTotalHeight > algListVisibleHeight && algScrollbarThumb.getGlobalBounds().contains(mousePos)) { /* ... */ 
                isDraggingAlgScrollbar = true; return;
            }
        }
    }
    if (e.type == sf::Event::MouseButtonReleased) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            isDraggingSpeedSlider = false; isDraggingAlgScrollbar = false; isDraggingDSSlider = false;
        }
    }
    if (e.type == sf::Event::MouseMoved) {
        if (isDraggingDSSlider) {
            if (dsListTotalWidth > dsListVisibleWidth && (dsSliderTrackRect.width - dsSliderKnobWidth) > 0) {
                 float relativeMouseX = mousePos.x - dsSliderTrackRect.left - dsSliderKnobWidth / 2.f;
                 float clickRatio = relativeMouseX / (dsSliderTrackRect.width - dsSliderKnobWidth);
                 clickRatio = std::clamp(clickRatio, 0.f, 1.f);
                 dsListScrollOffset = clickRatio * (dsListTotalWidth - dsListVisibleWidth);
                 updateDSSliderKnobPositionFromScrollOffset(); // This updates dsSliderKnobX
                 layoutDSCheckboxes(); // Update checkbox positions
            }
        }
        else if (isDraggingSpeedSlider) { /* ... same as before ... */ 
            float minXTrack = speedSliderPaneRect.left + PADDING + 10.f;
            float maxXTrack = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
            speedSliderKnobX = std::clamp(mousePos.x, minXTrack, maxXTrack);
            updateSpeedFromSliderKnobPosition();
        }
        else if (isDraggingAlgScrollbar) { /* ... same as before ... */ 
            float mouseYRelToScrollbarBack = mousePos.y - algScrollbarBack.getPosition().y;
            float scrollableThumbTravel = algListVisibleHeight - algScrollbarThumb.getSize().y;
            float thumbRatio = (scrollableThumbTravel > 0.f) ? std::clamp(mouseYRelToScrollbarBack / scrollableThumbTravel, 0.f, 1.f) : 0.f;
            float maxScroll = algListTotalHeight - algListVisibleHeight;
            algListScrollOffset = thumbRatio * max(0.f, maxScroll);
            layoutAlgorithmCheckboxes();
        }
    }
    if (e.type == sf::Event::MouseWheelScrolled) {
        // Check if mouse is over the DS item display area (not the whole topBarRect)
        sf::FloatRect dsItemsArea(topBarRect.left + PADDING, topBarRect.top + TITLE_AREA_HEIGHT, dsListVisibleWidth, CHECKBOX_ITEM_HEIGHT + PADDING);
        if (dsItemsArea.contains(mousePos) && dsListTotalWidth > dsListVisibleWidth) {
            dsListScrollOffset -= e.mouseWheelScroll.delta * DS_ITEM_MIN_WIDTH / 2.f; // Scroll sensitivity
            dsListScrollOffset = std::clamp(dsListScrollOffset, 0.f, max(0.f, dsListTotalWidth - dsListVisibleWidth));
            updateDSSliderKnobPositionFromScrollOffset(); // Update slider knob
            layoutDSCheckboxes(); // Update checkbox positions
        } else if (algorithmPaneRect.contains(mousePos) && algListTotalHeight > algListVisibleHeight) { /* ... same as before ... */ 
            algListScrollOffset -= e.mouseWheelScroll.delta * CHECKBOX_ITEM_HEIGHT * 2;
            algListScrollOffset = std::clamp(algListScrollOffset, 0.f, max(0.f, algListTotalHeight - algListVisibleHeight));
            layoutAlgorithmCheckboxes();
        }
    }
}

void GUI::draw() {
    renderer.drawPanel(topBarRect, "Data Structure", 18);
    renderer.drawPanel(visualizationPaneRect, "Visualization", 18);
    renderer.drawPanel(algorithmPaneRect, "Algorithms", 16);
    renderer.drawPanel(speedSliderPaneRect, "Speed", 16);
    renderer.drawPanel(complexityPaneRect, "Complexity Analysis", 16);

    // Draw DS Checkboxes (clipped by visibility logic)
    for (const auto& cb : dsCheckboxItems) {
        if (cb.isVisible) {
             bool isChecked = (visHandler.getCurrentDataStructure() == cb.dsEnum ||
                              (visHandler.getCurrentDataStructure() == DataStructure::Vector && cb.dsEnum == DataStructure::Array && cb.label == ARRAY_VECTOR_DISPLAY_NAME) ||
                              (visHandler.getCurrentDataStructure() == DataStructure::Array && cb.dsEnum == DataStructure::Array && cb.label == ARRAY_VECTOR_DISPLAY_NAME) );
             bool isHovered = cb.rect.contains(mousePos);
             // A more precise check if the item is truly within the designated scroll view area
             if (cb.rect.left + cb.rect.width > topBarRect.left + PADDING &&
                 cb.rect.left < topBarRect.left + PADDING + dsListVisibleWidth) {
                 // Adjust rect for drawing if it's partially visible for a smooth scroll effect
                 sf::FloatRect clippedRect = cb.rect;
                 float visibleLeft = topBarRect.left + PADDING;
                 float visibleRight = visibleLeft + dsListVisibleWidth;

                 if (clippedRect.left < visibleLeft) {
                     clippedRect.width -= (visibleLeft - clippedRect.left);
                     clippedRect.left = visibleLeft;
                 }
                 if (clippedRect.left + clippedRect.width > visibleRight) {
                     clippedRect.width = visibleRight - clippedRect.left;
                 }
                 if (clippedRect.width > 0) { // Only draw if some part is visible
                    renderer.drawCheckbox(clippedRect, cb.label, isChecked, isHovered, CHECKBOX_SQUARE_SIZE, CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X, CHECKBOX_FONT_SIZE);
                 }
             }
        }
    }
    // Draw DS Slider if scrollable
    if (dsListTotalWidth > dsListVisibleWidth) {
        renderer.drawHorizontalSlider(dsSliderTrackRect, dsSliderKnobX, dsSliderKnobWidth, dsSliderTrackRect.height * DS_SLIDER_KNOB_HEIGHT_FACTOR);
    }


    // Draw Algorithm Checkboxes & Scrollbar (same as before, with minor style consistency)
    if (algCheckboxItems.empty()) { /* ... */ 
        string msg = "No algorithms for " + visHandler.getCurrentDataStructureDisplayName();
        renderer.drawTextCentered(sf::FloatRect(algorithmPaneRect.left, algorithmPaneRect.top + TITLE_AREA_HEIGHT, algorithmPaneRect.width, algListVisibleHeight), msg, 14, Renderer::TEXT_MUTED_COLOR);
    } else { /* ... */ 
        for (const auto& cb : algCheckboxItems) {
            if (cb.rect.top + cb.rect.height > algorithmPaneRect.top + TITLE_AREA_HEIGHT &&
                cb.rect.top < algorithmPaneRect.top + TITLE_AREA_HEIGHT + algListVisibleHeight) {
                bool isChecked = (visHandler.getCurrentAlgorithm() == cb.algEnum);
                bool isHovered = cb.rect.contains(mousePos);
                renderer.drawCheckbox(cb.rect, cb.label, isChecked, isHovered, CHECKBOX_SQUARE_SIZE, CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X, CHECKBOX_FONT_SIZE);
            }
        }
    }
    if (algListTotalHeight > algListVisibleHeight && !algCheckboxItems.empty()) { /* ... */ 
        window.draw(algScrollbarBack); window.draw(algScrollbarThumb);
    }

    // Draw Speed Slider (label within panel)
    // renderer.drawText("Speed:", {speedSliderPaneRect.left + PADDING, speedSliderPaneRect.top + TITLE_AREA_HEIGHT - 20.f}, 14, Renderer::TEXT_COLOR);
    float sliderTrackY = speedSliderPaneRect.top + TITLE_AREA_HEIGHT + (speedSliderPaneRect.height - TITLE_AREA_HEIGHT - PADDING - 10.f) / 2.f; // 10 for text above
    float sliderTrackWidth = speedSliderPaneRect.width - 2 * (PADDING + 10.f);
    sf::FloatRect sliderActualTrack = {speedSliderPaneRect.left + PADDING + 10.f, sliderTrackY, sliderTrackWidth, 0.f};
    renderer.drawSlider(sliderActualTrack, speedSliderKnobX, 9.f); // Slightly larger knob
    
    // Draw Complexity Info & Graphs
    std::pair<string, string> complexity = compHandler.getComplexity(visHandler.getCurrentAlgorithm());
    renderer.drawFunctionGraph(timeComplexityGraphRect, complexity.first, Renderer::COMPLEXITY_TIME_COLOR, "Time");
    renderer.drawFunctionGraph(spaceComplexityGraphRect, complexity.second, Renderer::COMPLEXITY_SPACE_COLOR, "Space");
    
    float textComplexityY = spaceComplexityGraphRect.top + spaceComplexityGraphRect.height + PADDING / 2.f;
    renderer.drawText("Time: " + complexity.first, {complexityPaneRect.left + PADDING, textComplexityY}, 14, Renderer::TEXT_COLOR);
    renderer.drawText("Space: " + complexity.second, {complexityPaneRect.left + PADDING, textComplexityY + 18.f}, 14, Renderer::TEXT_COLOR);

    // Visualization Pane Content (same as before)
    if (visHandler.getCurrentAlgorithm() == Algorithm::None && !algCheckboxItems.empty()) { /* ... */ 
        renderer.drawTextCentered(visualizationPaneRect, "Select an Algorithm", 22, Renderer::TEXT_MUTED_COLOR);
    } else { /* ... */ 
        sf::FloatRect actualVizArea = {visualizationPaneRect.left + PADDING/2.f, visualizationPaneRect.top + TITLE_AREA_HEIGHT - PADDING/2.f,
                                   visualizationPaneRect.width - PADDING, visualizationPaneRect.height - TITLE_AREA_HEIGHT + PADDING/2.f};
        visHandler.drawCurrentVisualization(renderer, actualVizArea);
        string dsName = visHandler.getCurrentDataStructureDisplayName();
        string algName = (visHandler.getCurrentAlgorithm() != Algorithm::None && static_cast<size_t>(visHandler.getCurrentAlgorithm()) < ALGORITHM_NAMES.size()) ? ALGORITHM_NAMES[static_cast<int>(visHandler.getCurrentAlgorithm())] : "";
        string vizText = dsName;
        if (!algName.empty()) vizText += " / " + algName;
        else if (algCheckboxItems.empty() && visHandler.getCurrentDataStructure() != DataStructure::None) vizText += "\n(No specific algorithms)";
        vizText += "\n(Visualization Area)"; // Simpler placeholder
        renderer.drawTextCentered(actualVizArea, vizText, 20, Renderer::TEXT_MUTED_COLOR);
    }
}
