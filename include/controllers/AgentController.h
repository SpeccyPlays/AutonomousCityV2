#pragma once

#include "../objects/CityObject.h"
#include "../include/agents/Agent.h"

namespace AutoCity {
    class AgentController : public CityObject{
        private:
            std::vector<std::unique_ptr<AutoCity::Agent>> agents;
        public:
            AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
            void offGridHandler(const Event& e);
            void collisionHandler(const Event& e);
            void roadFlowHandler(const Event& e);
    };
};