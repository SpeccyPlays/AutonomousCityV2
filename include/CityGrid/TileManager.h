#pragma once
#include "../include/CityGrid/Tile.h"
#include <map>

namespace AutoCity {
    class TileManager {
        private:
            static std::map<TileType, std::map<TileSubType, Tile>> tiles;
            static sf::Texture* tileSheet;
        public:
            static void setTileSheet(sf::Texture* texture);
            static Tile getTile(TileType type, TileSubType subType);
    };
};