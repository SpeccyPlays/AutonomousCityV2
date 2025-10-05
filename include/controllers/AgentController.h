#pragma once

#include "../objects/CityObject.h"
#include "../include/agents/Agent.h"

namespace AutoCity {
    class AgentController : public CityObject{
        private:
            void agentAdded(const Event& e);
        public:
            AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
            
    };
};