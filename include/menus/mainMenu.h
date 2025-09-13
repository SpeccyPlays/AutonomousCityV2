#pragma once
#include "../include/CityObject.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp> 

namespace AutoCity {
    
    class MainMenu : public CityObject {
        
        private:
            void showMainMenu();
            void showFileOptions();
            void showToolsOptions();
        public:
            MainMenu(sf::RenderWindow& renderWindow);
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
    };   
};