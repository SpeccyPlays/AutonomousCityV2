#pragma once

#include <list>
#include <memory>
#include "../include/objects/CityObject.h"
#include "../include/CityGrid/Tile.h"

namespace AutoCity {
    class CityObjectController : public CityObject{
        private:
            std::list<std::unique_ptr<CityObject>> objects;
            std::unique_ptr<Tile> selectedTile = nullptr;
            bool tileSelected = false;
            void updateTileSelected(const Event& e);
            void deselectTile();
        public:
            CityObjectController(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
    };
};