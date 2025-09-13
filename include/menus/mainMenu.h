#pragma once

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp> 

namespace AutoCity {
    
    class MainMenu {
        
        private:
            sf::RenderWindow& window;
            void showMainMenu();
            void showFileOptions();
            void showToolsOptions();
        public:
            MainMenu(sf::RenderWindow& renderWindow);
            int init();
            void processEvents(const sf::Event& event);
            void update(sf::Time deltaClock);
            void draw();
    };   
};