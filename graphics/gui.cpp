// gui.cpp
#include "gui.h"
#include "types.h"
#include <algorithm>

GUI::GUI(sf::RenderWindow& win_, Renderer& r,
         VisualizationHandler& h, ComplexityHandler& c)
: win(win_), R(r), vh(h), comp(c)
{
    layoutCheckboxes();
}

void GUI::layoutCheckboxes() {
    // --- DS across topBar (two rows max)
    float x=topBar.left+12, y=topBar.top+28;
    for (size_t i=1;i<DATA_STRUCTURE_NAMES.size();++i) {
        dsRect.push_back({x,y,160,18});
        x+=170;
        if (x+160 > topBar.left+topBar.width-10){ x=topBar.left+12; y+=24;}
    }
    // --- Algorithm list in algoPane (scroll not needed yet)
    float ax = algoPane.left+12, ay=algoPane.top+44;
    for (size_t i=1;i<ALGORITHM_NAMES.size();++i) {
        algRect.push_back({ax,ay,170,18});
        ay+=22;
    }
}

void GUI::handleEvent(const sf::Event& e) {
    if (e.type==sf::Event::MouseButtonPressed) {
        sf::Vector2f m=(sf::Vector2f)sf::Mouse::getPosition(win);

        // slider knob?
        if (sliderBox.contains(m)) dragging=true;

        // --- DS clicks
        for (size_t i=0;i<dsRect.size();++i)
            if (dsRect[i].contains(m)) {
                DataStructure clicked=(DataStructure)(i+1);
                vh.setDataStructure(clicked); // always selects, never unselects
            }

        // --- Algo clicks
        for (size_t i=0;i<algRect.size();++i)
            if (algRect[i].contains(m)) {
                Algorithm clicked=(Algorithm)(i+1);
                if (vh.getAlgorithm()==clicked) vh.setAlgorithm(Algorithm::None);
                else                            vh.setAlgorithm(clicked);
            }
    }
    if (e.type==sf::Event::MouseButtonReleased) dragging=false;

    if (dragging && e.type==sf::Event::MouseMoved) {
        float minX=sliderBox.left+10, maxX=sliderBox.left+sliderBox.width-10;
        knobX = std::clamp((float)e.mouseMove.x, minX, maxX);
        int sp=(int)((maxX-knobX)/(maxX-minX)*1000);
        vh.setSpeedMs(sp);
    }
}

void GUI::draw() {
    // outer frame
    R.roundedRect(outer, sf::Color::Black, sf::Color::Transparent, 12.f);

    // --- top bar
    R.roundedRect(topBar, sf::Color::Black, sf::Color::Transparent);
    R.textCenter({topBar.left,topBar.top+2,topBar.width,24},
                 "Data Structure",20);

    for (size_t i=0;i<dsRect.size();++i) {
        bool checked = (vh.getDataStructure()==(DataStructure)(i+1));
        bool hover   = dsRect[i].contains((sf::Vector2f)sf::Mouse::getPosition(win));
        R.checkbox({dsRect[i].left,dsRect[i].top,18,18},
                   DATA_STRUCTURE_NAMES[i+1], checked, hover);
    }

    // --- main visualization border
    R.roundedRect(vizPane, sf::Color::Blue, sf::Color::Transparent);
    R.textCenter(vizPane,"Data Structure Visualized",18,sf::Color::Black);

    // --- algorithm pane
    R.roundedRect(algoPane, sf::Color::Black, sf::Color::Transparent);
    R.textCenter({algoPane.left,algoPane.top+4,algoPane.width,28},
                 "Algorithms",18);

    for (size_t i=0;i<algRect.size();++i) {
        bool checked = (vh.getAlgorithm()==(Algorithm)(i+1));
        bool hover   = algRect[i].contains((sf::Vector2f)sf::Mouse::getPosition(win));
        R.checkbox({algRect[i].left,algRect[i].top,18,18},
                   ALGORITHM_NAMES[i+1], checked, hover);
    }

    // --- slider box
    R.textCenter({sliderBox.left,sliderBox.top-18,sliderBox.width,18},
                 "Visualization Speed",16);
    R.slider({sliderBox.left+10,sliderBox.top+20,sliderBox.width-20,0}, knobX);

    // --- complexity box
    R.roundedRect(complexBox, sf::Color::Blue, sf::Color::Transparent);
    R.textCenter(complexBox,"Complexity Chart",16);
}
