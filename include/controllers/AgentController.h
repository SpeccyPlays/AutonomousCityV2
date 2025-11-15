#pragma once

#include "../objects/CityObject.h"
#include "../include/agents/Agent.h"
#include "../include/CityGrid/TileManager.h"

namespace AutoCity {
    class AgentController : public CityObject{
        private:
            std::vector<std::unique_ptr<AutoCity::Agent>> agents;
            void offGridHandler(Agent* agent, std::array<bool, 4> offGrid);
            void collisionHandler(const Event& e);
            void roadFlowHandler(const Event& e);
            void toggleAllDebug();
            void handleLookAheadBoundryCheck(const Event& e);
            void handleDesiredBoundryCheck(const Event& e);
            void tileHandler(const Event& e);
            float getTileFlowAngle(Tile tile, sf::Vector2f pos);
            void tileAngleActions(Agent* agent, float tileAngle);
            float wrapAngle(float angle);
            void saveAgents(const Event& e);
            void loadAgents(const Event& e);
        public:
            AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
    };
};