// graphics/gui.h
#pragma once
#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "visualization_handler.h"
#include "complexity_handler.h"
#include "../utilities/types.h"
#include <vector>
#include <string>
#include <set>

class GUI {
public:
    GUI(sf::RenderWindow& win, Renderer& r,
        VisualizationHandler& vh, ComplexityHandler& ch, sf::Font& font);

    void handleEvent(const sf::Event& e);
    void draw();

private:
    sf::RenderWindow& window;
    Renderer& renderer;
    VisualizationHandler& visHandler;
    ComplexityHandler& compHandler;
    sf::Font& appFont;

    const float PADDING = 15.f;
    const float TITLE_AREA_HEIGHT = 35.f; // Increased space for panel titles
    const float CHECKBOX_ITEM_HEIGHT = 24.f; // Slightly taller items
    const int CHECKBOX_FONT_SIZE = 14;
    const float CHECKBOX_TEXT_OFFSET_X = 8.f;
    const float CHECKBOX_SQUARE_SIZE = 16.f;
    const float DS_ITEM_MIN_WIDTH = 130.f;
    const float DS_ITEM_SPACING = 10.f;
    
    // DS Top Bar Slider
    const float DS_SLIDER_TRACK_HEIGHT = 8.f;
    const float DS_SLIDER_KNOB_HEIGHT_FACTOR = 1.5f; // Knob is taller than track

    sf::FloatRect topBarRect;
    sf::FloatRect visualizationPaneRect;
    sf::FloatRect algorithmPaneRect;
    sf::FloatRect speedSliderPaneRect;
    sf::FloatRect complexityPaneRect;
    sf::FloatRect timeComplexityGraphRect;
    sf::FloatRect spaceComplexityGraphRect;

    // DS Top Bar Slider
    float dsListScrollOffset = 0.f;
    float dsListVisibleWidth = 0.f;
    float dsListTotalWidth = 0.f;
    sf::FloatRect dsSliderTrackRect; // For the track of the DS horizontal slider
    float dsSliderKnobX = 0.f;       // Current X position of the DS slider knob
    float dsSliderKnobWidth = 50.f;  // Width of the DS slider knob
    bool isDraggingDSSlider = false;


    float algListScrollOffset = 0.f;
    float algListVisibleHeight = 0.f;
    float algListTotalHeight = 0.f;
    sf::RectangleShape algScrollbarBack;
    sf::RectangleShape algScrollbarThumb;
    bool isDraggingAlgScrollbar = false;

    bool isDraggingSpeedSlider = false;
    float speedSliderKnobX;

    struct CheckboxUIData {
        sf::FloatRect rect;
        std::string label;
        DataStructure dsEnum;
        Algorithm algEnum;
        bool isDS;
        bool isVisible = true;
    };
    std::vector<CheckboxUIData> dsCheckboxItems;
    std::vector<CheckboxUIData> algCheckboxItems;
    
    sf::Vector2f mousePos;

    void initializeLayout(unsigned int windowWidth, unsigned int windowHeight);
    void populateDSCheckboxes();
    void layoutDSCheckboxes(); // This will now use dsListScrollOffset
    void populateAlgorithmCheckboxes();
    void layoutAlgorithmCheckboxes();

    void updateSliderKnobPositionFromSpeed();
    void updateSpeedFromSliderKnobPosition();

    // New methods for DS Slider
    void updateDSSliderKnobPositionFromScrollOffset();
    void updateDSScrollOffsetFromSliderKnobPosition();
};
