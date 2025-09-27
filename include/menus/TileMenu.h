#pragma once
#include "../include/objects/CityObject.h"
#include "../include/CityGrid/Tile.h"
#include "../include/CityGrid/TileManager.h"
#include "../EventBus.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace AutoCity {
    
    class TileMenu : public CityObject {
        private:
            std::vector<Tile> allTiles;
            void loadTiles(const Event& e);
            std::vector<sf::Angle> rotateFlowMap(std::vector<sf::Angle> map, int angle);
            void showTileMenu();
        public:
            TileMenu(sf::RenderWindow& renderWindow, EventBus& eventBus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
    };
};