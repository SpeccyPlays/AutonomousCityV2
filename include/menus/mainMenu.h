#pragma once
#include "../include/menus/menu.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp> 

namespace AutoCity {
    
    class MainMenu : public Menu {
        
        private:
            sf::RenderWindow& window;
            void showMainMenu();
            void showFileOptions();
            void showToolsOptions();
        public:
            MainMenu(sf::RenderWindow& renderWindow);
            void processEvents(const sf::Event& event) override;
            void update() override;
    };   
};