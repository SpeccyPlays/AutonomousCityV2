#pragma once

#include <list>
#include <memory>
#include "../include/objects/CityObject.h"

namespace AutoCity {
    class CityObjectController : public CityObject{
        private:
            std::list<std::unique_ptr<CityObject>> objects;
        public:
            CityObjectController(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
    };
};