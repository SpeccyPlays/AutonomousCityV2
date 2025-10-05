#pragma once

#include "../include/objects/CityObject.h"

namespace AutoCity {
    class Agent : public CityObject{
        private:

        public:
            Agent(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
            
    };
};