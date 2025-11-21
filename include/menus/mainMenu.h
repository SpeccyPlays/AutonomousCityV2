#pragma once
#include "../include/objects/CityObject.h"
#include "../EventBus.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../nfdExtended/nfd.h" 
#include <stdio.h>
#include <stdlib.h>

namespace AutoCity {
    
    class MainMenu : public CityObject {
        
        private:
            void showMainMenu();
            void showFileOptions();
            void showToolsOptions();
            bool debugGrid = true; //make sure in the CityGridController that this is toggled in the init as off for all CityObjects by default;
            bool debugAgents = false;
            void fileOpenHandler();
            void fileSaveAsHandler();
        public:
            MainMenu(sf::RenderWindow& renderWindow, EventBus& eventBus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
    };   
};