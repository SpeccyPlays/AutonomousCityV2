#pragma once

#include "../include/objects/CityObject.h"
#include "../include/CityGrid/Tile.h"
#include "../include/CityGrid/TileManager.h"

namespace AutoCity {
    class CityGridController : public CityObject{
        private:
            sf::Vector2i gridStart;
            sf::Vector2i gridEnd;
            sf::Vector2i gridSize;
            std::vector<std::vector<Tile>> grid;
            void newGrid();
            void newGrid(const Event& e);
            void drawGrid();
            void drawTiles();
            void addTile(const Event& e);
            bool checkInGrid(sf::Vector2u pos);
            void addTileToGrid(sf::Vector2u pos, Tile tile);
            sf::Vector2u pixelToGridPos(sf::Vector2u pos);
        public:
            CityGridController(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
            
    };
};