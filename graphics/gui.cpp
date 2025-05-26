// graphics/gui.cpp
#include "gui.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <set> // For unique DS names

// Using declarations for std types in .cpp file
using std::string;
using std::vector;
using std::set;
using std::max;
using std::min;
// No custom namespace for av::GUI

GUI::GUI(sf::RenderWindow& win_, Renderer& r_,
         VisualizationHandler& vh_, ComplexityHandler& ch_, sf::Font& font_)
: window(win_), renderer(r_), visHandler(vh_), compHandler(ch_), appFont(font_) {
    initializeLayout(window.getSize().x, window.getSize().y);
    populateDSCheckboxes();
    layoutDSCheckboxes();
    populateAlgorithmCheckboxes();
    // layoutAlgorithmCheckboxes(); // Already called by populateAlgorithmCheckboxes
    updateSliderKnobPositionFromSpeed();
}

void GUI::initializeLayout(unsigned int windowWidth, unsigned int windowHeight) {
    float rightPaneWidth = 280.f;
    float dsBarHeight = 60.f;

    topBarRect = {PADDING, PADDING, windowWidth - 2 * PADDING, dsBarHeight};
    dsListVisibleWidth = topBarRect.width - 2 * TOP_BAR_SCROLL_BUTTON_WIDTH - 2 * PADDING;

    visualizationPaneRect = {PADDING, topBarRect.top + topBarRect.height + PADDING,
                             windowWidth - rightPaneWidth - 3 * PADDING,
                             windowHeight - (topBarRect.top + topBarRect.height + PADDING) - PADDING};

    float rightPaneTotalHeight = windowHeight - (topBarRect.top + topBarRect.height + PADDING) - PADDING;
    float algoPaneHeight = rightPaneTotalHeight * 0.30f;
    float speedSliderHeight = 70.f;
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
    float availableGraphHeight = (complexityPaneRect.height - TITLE_HEIGHT - complexityTextHeight - 2 * graphTitleMargin - 2 * PADDING ) / 2.f;
    
    timeComplexityGraphRect = {complexityPaneRect.left + PADDING,
                               complexityPaneRect.top + TITLE_HEIGHT + PADDING,
                               complexityPaneRect.width - 2 * PADDING,
                               max(50.f, availableGraphHeight)};

    spaceComplexityGraphRect = {complexityPaneRect.left + PADDING,
                                timeComplexityGraphRect.top + timeComplexityGraphRect.height + PADDING + graphTitleMargin,
                                complexityPaneRect.width - 2 * PADDING,
                                max(50.f, availableGraphHeight)};

    algListVisibleHeight = algorithmPaneRect.height - TITLE_HEIGHT - PADDING;
    algScrollbarBack.setSize({10.f, algListVisibleHeight});
    algScrollbarBack.setFillColor(sf::Color(50,50,50));
    algScrollbarBack.setPosition(algorithmPaneRect.left + algorithmPaneRect.width - PADDING - 10.f,
                                 algorithmPaneRect.top + TITLE_HEIGHT);
    
    dsScrollLeftButton.setSize({TOP_BAR_SCROLL_BUTTON_WIDTH, dsBarHeight - TITLE_HEIGHT});
    dsScrollLeftButton.setFillColor(Renderer::PRIMARY_UI_COLOR); // Use static color from Renderer
    dsScrollLeftButton.setPosition(topBarRect.left + PADDING/2.f, topBarRect.top + TITLE_HEIGHT/2.f);

    dsScrollRightButton.setSize({TOP_BAR_SCROLL_BUTTON_WIDTH, dsBarHeight - TITLE_HEIGHT});
    dsScrollRightButton.setFillColor(Renderer::PRIMARY_UI_COLOR);
    dsScrollRightButton.setPosition(topBarRect.left + topBarRect.width - TOP_BAR_SCROLL_BUTTON_WIDTH - PADDING/2.f, topBarRect.top + TITLE_HEIGHT/2.f);
}

void GUI::populateDSCheckboxes() {
    dsCheckboxItems.clear(); // Corrected variable name
    dsListTotalWidth = 0;
    set<string> displayedNames;

    for (int i = 1; i < static_cast<int>(DataStructure::SegmentTree) + 1; ++i) {
        DataStructure currentEnum = static_cast<DataStructure>(i);
        string name = DATA_STRUCTURE_NAMES[i]; // Global DATA_STRUCTURE_NAMES

        if (currentEnum == DataStructure::Vector) continue;

        if (displayedNames.find(name) == displayedNames.end() || name == ARRAY_VECTOR_DISPLAY_NAME) {
             if (name == ARRAY_VECTOR_DISPLAY_NAME && displayedNames.count(name)) continue;

            displayedNames.insert(name);
            
            sf::Text tempText(name, appFont, CHECKBOX_FONT_SIZE);
            float textWidth = tempText.getLocalBounds().width;
            float itemWidth = CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X + textWidth + PADDING * 1.5f;
            itemWidth = max(itemWidth, DS_ITEM_MIN_WIDTH);

            dsCheckboxItems.push_back({ // Corrected variable name
                {0,0,0,0}, name, currentEnum, Algorithm::None, true, true
            });
            dsListTotalWidth += itemWidth + DS_ITEM_SPACING; // Using const DS_ITEM_SPACING
        }
    }
    if (!dsCheckboxItems.empty()) dsListTotalWidth -= DS_ITEM_SPACING; // Corrected variable name
}

void GUI::layoutDSCheckboxes() {
    float currentX = topBarRect.left + TOP_BAR_SCROLL_BUTTON_WIDTH + PADDING - dsListScrollOffset;
    float yPos = topBarRect.top + TITLE_HEIGHT;

    for (CheckboxUIData& cb : dsCheckboxItems) { // Corrected variable name
        sf::Text tempText(cb.label, appFont, CHECKBOX_FONT_SIZE);
        float textWidth = tempText.getLocalBounds().width;
        float itemWidth = CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X + textWidth + PADDING * 1.5f;
        itemWidth = max(itemWidth, DS_ITEM_MIN_WIDTH);

        cb.rect = {currentX, yPos, itemWidth, CHECKBOX_ITEM_HEIGHT};
        cb.isVisible = (cb.rect.left + cb.rect.width > topBarRect.left + TOP_BAR_SCROLL_BUTTON_WIDTH + PADDING / 2.f &&
                        cb.rect.left < topBarRect.left + TOP_BAR_SCROLL_BUTTON_WIDTH + PADDING + dsListVisibleWidth - PADDING / 2.f);
        currentX += itemWidth + DS_ITEM_SPACING; // Using const DS_ITEM_SPACING
    }
}

void GUI::populateAlgorithmCheckboxes() {
    algCheckboxItems.clear(); // Corrected variable name
    algListTotalHeight = 0;
    vector<Algorithm> availableAlgs = visHandler.getAvailableAlgorithms();

    if (availableAlgs.empty()) {
        // Message shown in draw()
    } else {
        for (Algorithm algEnum : availableAlgs) { // Algorithm is global enum
            if (static_cast<size_t>(algEnum) < ALGORITHM_NAMES.size()) { // ALGORITHM_NAMES is global
                algCheckboxItems.push_back({ // Corrected variable name
                    {0,0,0,0}, ALGORITHM_NAMES[static_cast<int>(algEnum)],
                    DataStructure::None, algEnum, false, true
                });
                algListTotalHeight += CHECKBOX_ITEM_HEIGHT;
            }
        }
    }
    algListScrollOffset = 0;
    layoutAlgorithmCheckboxes(); // Corrected method name
}

// Definition for the corrected method name
void GUI::layoutAlgorithmCheckboxes() {
    float currentY_Alg = algorithmPaneRect.top + TITLE_HEIGHT - algListScrollOffset;
    float xPosAlg = algorithmPaneRect.left + PADDING;
    float algItemWidth = algorithmPaneRect.width - 2 * PADDING - (algListTotalHeight > algListVisibleHeight ? 15.f : 0.f);

    for (CheckboxUIData& cbInfo : algCheckboxItems) { // Corrected variable name
        cbInfo.rect = {xPosAlg, currentY_Alg, algItemWidth, CHECKBOX_ITEM_HEIGHT};
        currentY_Alg += CHECKBOX_ITEM_HEIGHT;
    }

    if (algListTotalHeight > algListVisibleHeight) {
        float thumbHeightRatio = algListVisibleHeight / algListTotalHeight;
        float thumbHeight = max(20.f, algListVisibleHeight * thumbHeightRatio);
        algScrollbarThumb.setSize({10.f, thumbHeight});
        algScrollbarThumb.setFillColor(sf::Color(100,100,100));
        float scrollableDist = algListTotalHeight - algListVisibleHeight;
        float scrollOffsetRatio = (scrollableDist > 0.f) ? (algListScrollOffset / scrollableDist) : 0.f;
        float thumbMaxYTravel = algListVisibleHeight - thumbHeight;
        algScrollbarThumb.setPosition(algScrollbarBack.getPosition().x,
                                      algScrollbarBack.getPosition().y + scrollOffsetRatio * thumbMaxYTravel);
    }
}


void GUI::updateSliderKnobPositionFromSpeed() {
    float minX = speedSliderPaneRect.left + PADDING + 10.f;
    float maxX = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
    float trackWidth = maxX - minX;
    if (trackWidth <= 0) return;
    float maxSpeed = 2000.f;
    float currentSpeed = static_cast<float>(visHandler.getSpeedMs());
    speedSliderKnobX = minX + (currentSpeed / maxSpeed) * trackWidth;
    speedSliderKnobX = std::clamp(speedSliderKnobX, minX, maxX);
}

void GUI::updateSpeedFromSliderKnobPosition() {
    float minX = speedSliderPaneRect.left + PADDING + 10.f;
    float maxX = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
    float trackWidth = maxX - minX;
    if (trackWidth <= 0) return;
    float ratio = (speedSliderKnobX - minX) / trackWidth;
    float maxSpeed = 2000.f;
    int newSpeed = static_cast<int>(std::round(ratio * maxSpeed));
    newSpeed = std::clamp(newSpeed, 0, static_cast<int>(maxSpeed));
    visHandler.setSpeedMs(newSpeed);
}

void GUI::handleEvent(const sf::Event& e) {
    mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    if (e.type == sf::Event::MouseButtonPressed) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            for (const auto& cb : dsCheckboxItems) { // Corrected variable name
                if (cb.isVisible && cb.rect.contains(mousePos)) {
                    visHandler.setDataStructure(cb.dsEnum);
                    populateAlgorithmCheckboxes();
                    return;
                }
            }
            if (dsScrollLeftButton.getGlobalBounds().contains(mousePos)) {
                dsListScrollOffset -= DS_ITEM_MIN_WIDTH;
                dsListScrollOffset = max(0.f, dsListScrollOffset);
                layoutDSCheckboxes(); return;
            }
            if (dsScrollRightButton.getGlobalBounds().contains(mousePos)) {
                float maxScroll = dsListTotalWidth - dsListVisibleWidth;
                dsListScrollOffset += DS_ITEM_MIN_WIDTH;
                dsListScrollOffset = min(dsListScrollOffset, max(0.f, maxScroll));
                layoutDSCheckboxes(); return;
            }

            for (const auto& cb : algCheckboxItems) { // Corrected variable name
                if (cb.rect.top + cb.rect.height > algorithmPaneRect.top + TITLE_HEIGHT &&
                    cb.rect.top < algorithmPaneRect.top + TITLE_HEIGHT + algListVisibleHeight &&
                    cb.rect.contains(mousePos)) {
                    visHandler.setAlgorithm(cb.algEnum); return;
                }
            }

            sf::FloatRect sliderTrackClickArea( speedSliderPaneRect.left + PADDING, speedSliderPaneRect.top + TITLE_HEIGHT,
                speedSliderPaneRect.width - 2 * PADDING, speedSliderPaneRect.height - TITLE_HEIGHT - PADDING);
            if (sliderTrackClickArea.contains(mousePos)) {
                 isDraggingSpeedSlider = true;
                 float minXTrack = speedSliderPaneRect.left + PADDING + 10.f;
                 float maxXTrack = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
                 speedSliderKnobX = std::clamp(mousePos.x, minXTrack, maxXTrack);
                 updateSpeedFromSliderKnobPosition(); return;
            }

            if (algListTotalHeight > algListVisibleHeight && algScrollbarThumb.getGlobalBounds().contains(mousePos)) {
                isDraggingAlgScrollbar = true; return;
            }
        }
    }
    if (e.type == sf::Event::MouseButtonReleased) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            isDraggingSpeedSlider = false; isDraggingAlgScrollbar = false;
        }
    }
    if (e.type == sf::Event::MouseMoved) {
        if (isDraggingSpeedSlider) {
            float minXTrack = speedSliderPaneRect.left + PADDING + 10.f;
            float maxXTrack = speedSliderPaneRect.left + speedSliderPaneRect.width - PADDING - 10.f;
            speedSliderKnobX = std::clamp(mousePos.x, minXTrack, maxXTrack);
            updateSpeedFromSliderKnobPosition();
        }
        if (isDraggingAlgScrollbar) {
            float mouseYRelToScrollbarBack = mousePos.y - algScrollbarBack.getPosition().y;
            float scrollableThumbTravel = algListVisibleHeight - algScrollbarThumb.getSize().y;
            float thumbRatio = (scrollableThumbTravel > 0.f) ? std::clamp(mouseYRelToScrollbarBack / scrollableThumbTravel, 0.f, 1.f) : 0.f;
            float maxScroll = algListTotalHeight - algListVisibleHeight;
            algListScrollOffset = thumbRatio * max(0.f, maxScroll);
            layoutAlgorithmCheckboxes(); // Corrected method name
        }
    }
    if (e.type == sf::Event::MouseWheelScrolled) {
        if (topBarRect.contains(mousePos) && dsListTotalWidth > dsListVisibleWidth) {
            dsListScrollOffset -= e.mouseWheelScroll.delta * DS_ITEM_MIN_WIDTH / 1.5f;
            dsListScrollOffset = std::clamp(dsListScrollOffset, 0.f, max(0.f, dsListTotalWidth - dsListVisibleWidth));
            layoutDSCheckboxes();
        } else if (algorithmPaneRect.contains(mousePos) && algListTotalHeight > algListVisibleHeight) {
            algListScrollOffset -= e.mouseWheelScroll.delta * CHECKBOX_ITEM_HEIGHT * 2;
            algListScrollOffset = std::clamp(algListScrollOffset, 0.f, max(0.f, algListTotalHeight - algListVisibleHeight));
            layoutAlgorithmCheckboxes(); // Corrected method name
        }
    }
}

void GUI::draw() {
    renderer.drawPanel(topBarRect, "Data Structure");
    renderer.drawPanel(visualizationPaneRect);
    renderer.drawPanel(algorithmPaneRect, "Algorithms");
    renderer.drawPanel(speedSliderPaneRect, "Visualization Speed");
    renderer.drawPanel(complexityPaneRect, "Complexity Analysis");

    if (dsListTotalWidth > dsListVisibleWidth) {
        window.draw(dsScrollLeftButton); window.draw(dsScrollRightButton);
        renderer.drawTextCentered(dsScrollLeftButton.getGlobalBounds(), "<", 18, Renderer::TEXT_COLOR);
        renderer.drawTextCentered(dsScrollRightButton.getGlobalBounds(), ">", 18, Renderer::TEXT_COLOR);
    }

    for (const auto& cb : dsCheckboxItems) { // Corrected variable name
        if (cb.isVisible) {
             bool isChecked = (visHandler.getCurrentDataStructure() == cb.dsEnum ||
                              (visHandler.getCurrentDataStructure() == DataStructure::Vector && cb.dsEnum == DataStructure::Array && cb.label == ARRAY_VECTOR_DISPLAY_NAME) ||
                              (visHandler.getCurrentDataStructure() == DataStructure::Array && cb.dsEnum == DataStructure::Array && cb.label == ARRAY_VECTOR_DISPLAY_NAME) );
             bool isHovered = cb.rect.contains(mousePos);
             if (cb.rect.left + cb.rect.width > topBarRect.left + TOP_BAR_SCROLL_BUTTON_WIDTH &&
                 cb.rect.left < topBarRect.left + TOP_BAR_SCROLL_BUTTON_WIDTH + dsListVisibleWidth + PADDING) {
                renderer.drawCheckbox(cb.rect, cb.label, isChecked, isHovered, CHECKBOX_SQUARE_SIZE, CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X, CHECKBOX_FONT_SIZE);
             }
        }
    }

    if (algCheckboxItems.empty()) { // Corrected variable name
        string msg = "No algorithms for " + visHandler.getCurrentDataStructureDisplayName();
        renderer.drawTextCentered(sf::FloatRect(algorithmPaneRect.left, algorithmPaneRect.top + TITLE_HEIGHT, algorithmPaneRect.width, algListVisibleHeight), msg, 14, Renderer::TEXT_COLOR);
    } else {
        for (const auto& cb : algCheckboxItems) { // Corrected variable name
            if (cb.rect.top + cb.rect.height > algorithmPaneRect.top + TITLE_HEIGHT &&
                cb.rect.top < algorithmPaneRect.top + TITLE_HEIGHT + algListVisibleHeight) {
                bool isChecked = (visHandler.getCurrentAlgorithm() == cb.algEnum);
                bool isHovered = cb.rect.contains(mousePos);
                renderer.drawCheckbox(cb.rect, cb.label, isChecked, isHovered, CHECKBOX_SQUARE_SIZE, CHECKBOX_SQUARE_SIZE + CHECKBOX_TEXT_OFFSET_X, CHECKBOX_FONT_SIZE);
            }
        }
    }
    if (algListTotalHeight > algListVisibleHeight && !algCheckboxItems.empty()) { // Corrected variable name
        window.draw(algScrollbarBack); window.draw(algScrollbarThumb);
    }

    float sliderTrackY = speedSliderPaneRect.top + TITLE_HEIGHT + (speedSliderPaneRect.height - TITLE_HEIGHT - PADDING) / 2.f;
    float sliderTrackWidth = speedSliderPaneRect.width - 2 * (PADDING + 10.f);
    sf::FloatRect sliderActualTrack = {speedSliderPaneRect.left + PADDING + 10.f, sliderTrackY, sliderTrackWidth, 0.f};
    renderer.drawSlider(sliderActualTrack, speedSliderKnobX);
    
    std::pair<string, string> complexity = compHandler.getComplexity(visHandler.getCurrentAlgorithm()); // std::pair
    renderer.drawFunctionGraph(timeComplexityGraphRect, complexity.first, Renderer::COMPLEXITY_TIME_COLOR, "Time");
    renderer.drawFunctionGraph(spaceComplexityGraphRect, complexity.second, Renderer::COMPLEXITY_SPACE_COLOR, "Space");
    
    float textComplexityY = spaceComplexityGraphRect.top + spaceComplexityGraphRect.height + PADDING;
    renderer.drawText("Time: " + complexity.first, {complexityPaneRect.left + PADDING, textComplexityY}, 15, Renderer::TEXT_COLOR);
    renderer.drawText("Space: " + complexity.second, {complexityPaneRect.left + PADDING, textComplexityY + 20.f}, 15, Renderer::TEXT_COLOR);

    if (visHandler.getCurrentAlgorithm() == Algorithm::None && !algCheckboxItems.empty()) { // Corrected variable name
        renderer.drawTextCentered(visualizationPaneRect, "Select an Algorithm", 20, Renderer::TEXT_COLOR);
    } else {
        sf::FloatRect actualVizArea = {visualizationPaneRect.left + 5.f, visualizationPaneRect.top + TITLE_HEIGHT/2.f + 5.f,
                                   visualizationPaneRect.width - 10.f, visualizationPaneRect.height - TITLE_HEIGHT/2.f - 10.f};
        visHandler.drawCurrentVisualization(renderer, actualVizArea);
        string dsName = visHandler.getCurrentDataStructureDisplayName();
        string algName = (visHandler.getCurrentAlgorithm() != Algorithm::None && static_cast<size_t>(visHandler.getCurrentAlgorithm()) < ALGORITHM_NAMES.size()) ? ALGORITHM_NAMES[static_cast<int>(visHandler.getCurrentAlgorithm())] : "";
        string vizText = dsName;
        if (!algName.empty()) vizText += " / " + algName;
        else if (algCheckboxItems.empty()) vizText += "\n(No specific algorithms)"; // Corrected variable name
        vizText += "\n(Visualization not implemented yet)";
        renderer.drawTextCentered(actualVizArea, vizText, 18, Renderer::TEXT_COLOR);
    }
}
