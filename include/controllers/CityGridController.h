#pragma once

#include "../include/objects/CityObject.h"
#include "../include/CityGrid/Tile.h"
#include "../include/CityGrid/TileManager.h"
#include "../include/agents/Agent.h"
#include <unordered_set>

namespace AutoCity {
    class CityGridController : public CityObject{
        public:
            struct Cell {
                Tile tile;
                std::unordered_set<AutoCity::Agent *> occupants;
            };
            CityGridController(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
            
        private:
            sf::Vector2i gridStart;
            sf::Vector2i gridEnd;
            sf::Vector2u gridSize;
            std::vector<std::vector<Cell>> grid;
            void newGrid();
            void newGrid(const Event& e);
            void drawGrid();
            void drawTiles();
            void addTile(const Event& e);
            bool checkInGrid(sf::Vector2f pos);
            void addTileToGrid(sf::Vector2f pos, Tile tile);
            sf::Vector2u pixelToGridPos(sf::Vector2f pos);
            void removeAgent(const Event& e);
            void addAgent(Agent *agent, sf::Vector2u agentGridPos);
            void agentUpdate(const Event& e);
            std::array<bool, 4> isAgentOnGrid(Agent *agent, sf::Vector2f agentPos);
            void agentLookAhead(const Event& e);
            void agentDesiredBoundaryCheck(const Event& e);
            void sendOccupants(const Event& e);
            void sendTileForAgent(const Event& e);
    };
};