#include "../include/CityGrid/TileManager.h"

namespace AutoCity {
    
    std::map<TileType, std::map<TileSubType, Tile>> TileManager::tiles;
    sf::Texture* TileManager::tileSheet = nullptr;
    
    void TileManager::setTileSheet(sf::Texture* texture){
        tileSheet = texture;
    };

   Tile TileManager::getTile(TileType type, TileSubType subType){
        auto firstMap = tiles.find(type);
        if (firstMap != tiles.end()){
            auto& subMap = firstMap->second;
            auto tile = subMap.find(subType);
            if (tile != subMap.end()){
                return tile->second;
            }
        }
        return Tile{};
    };
};