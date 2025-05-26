// graphics/gui.h
#pragma once
#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "visualization_handler.h"
#include "complexity_handler.h"
#include "../utilities/types.h" // Includes global enums
#include <vector>
#include <string>
#include <set>

// Class is now in the global namespace

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

    // Constants are part of the class or global, not a separate namespace
    const float PADDING = 15.f;
    const float TITLE_HEIGHT = 30.f;
    const float CHECKBOX_ITEM_HEIGHT = 22.f;
    const int   CHECKBOX_FONT_SIZE = 14;
    const float CHECKBOX_TEXT_OFFSET_X = 5.f; // Renamed for clarity
    const float CHECKBOX_SQUARE_SIZE = 15.f;
    const float DS_ITEM_MIN_WIDTH = 120.f;
    const float DS_ITEM_SPACING = 10.f; // Corrected: Was DS_ITEM_SPACING, now accessible
    const float TOP_BAR_SCROLL_BUTTON_WIDTH = 25.f;

    sf::FloatRect topBarRect;
    sf::FloatRect visualizationPaneRect;
    sf::FloatRect algorithmPaneRect;
    sf::FloatRect speedSliderPaneRect;
    sf::FloatRect complexityPaneRect;
    sf::FloatRect timeComplexityGraphRect;
    sf::FloatRect spaceComplexityGraphRect;

    float dsListScrollOffset = 0.f;
    float dsListVisibleWidth = 0.f;
    float dsListTotalWidth = 0.f;
    sf::RectangleShape dsScrollLeftButton;
    sf::RectangleShape dsScrollRightButton;

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
        DataStructure dsEnum; // Using global DataStructure enum
        Algorithm algEnum;    // Using global Algorithm enum
        bool isDS;
        bool isVisible = true;
    };
    std::vector<CheckboxUIData> dsCheckboxItems; // Corrected name
    std::vector<CheckboxUIData> algCheckboxItems; // Corrected name
    
    sf::Vector2f mousePos;

    void initializeLayout(unsigned int windowWidth, unsigned int windowHeight);
    void populateDSCheckboxes();
    void layoutDSCheckboxes();
    void populateAlgorithmCheckboxes();
    void layoutAlgorithmCheckboxes(); // Corrected name

    void updateSliderKnobPositionFromSpeed();
    void updateSpeedFromSliderKnobPosition();
};
