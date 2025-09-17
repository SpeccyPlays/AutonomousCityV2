#pragma once
#include "../include/CityGrid/Tile.h"
#include "../include/EventBus.h"
#include <map>

namespace AutoCity {
    class TileManager {
        private:
            static std::map<TileType, std::map<TileSubType, Tile>> tiles;
            static EventBus* eventBus;
        public:
            static void setEventBus(EventBus& bus);
            static void setBasicTiles(sf::Texture& tex);
            static Tile getTile(TileType type, TileSubType subType);
            static std::map<TileType, std::map<TileSubType, Tile>>& getTiles();
    };
};