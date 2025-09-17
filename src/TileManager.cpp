#include "../include/CityGrid/TileManager.h"

namespace AutoCity {
    
    std::map<TileType, std::map<TileSubType, Tile>> TileManager::tiles;
    EventBus* TileManager::eventBus = nullptr;
    
    void TileManager::setEventBus(EventBus& bus){
        eventBus = &bus;
    }

    void TileManager::setBasicTiles(sf::Texture texture){
        /*
        Straight,
        LeftTurn,
        RightTurn,
        DeadEnd,
        TJunction
        */
        sf::Angle north = sf::degrees(270);
        sf::Angle south = sf::degrees(90);
        sf::Angle east = sf::degrees(0);
        sf::Angle west = sf::degrees(180);
        /*
            *****Flow map guide****  
            Top left of tile is first value
            Top right is second value
            Bottom right is third value
            Bottom left is last value
       */
        int tileSize = 32;
        sf::Sprite roadStraightSprite(texture, sf::IntRect({0, 0}, {tileSize, tileSize}));
        Tile roadStraight = {
            TileType::Road, 
            TileSubType::Straight, 
            roadStraightSprite, 
            {north, south, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road][TileSubType::Straight] = roadStraight;

        sf::Sprite leftTurnSprite(texture, sf::IntRect({tileSize, 0}, {tileSize, tileSize}));
        Tile leftTurn = {
            TileType::Road, 
            TileSubType::LeftTurn, 
            leftTurnSprite, 
            {east, south, south, west}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road][TileSubType::LeftTurn] = leftTurn;

        sf::Sprite rightTurnSprite(texture, sf::IntRect({tileSize * 2, 0}, {tileSize, tileSize}));
        Tile rightTurn = {
            TileType::Road, 
            TileSubType::RightTurn, 
            rightTurnSprite, 
            {east, east, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road][TileSubType::RightTurn] = rightTurn;

        sf::Sprite tJuncSprite(texture, sf::IntRect({tileSize * 3, 0}, {tileSize, tileSize}));
        Tile tJunction = {
            TileType::Road, 
            TileSubType::TJunction, 
            tJuncSprite, 
            {east, east, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road][TileSubType::TJunction] = tJunction;

        sf::Sprite deadEndSprite(texture, sf::IntRect({tileSize * 4, 0}, {tileSize, tileSize}));
        Tile deadEnd = {
            TileType::Road, 
            TileSubType::DeadEnd, 
            deadEndSprite, 
            {east, east, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road][TileSubType::DeadEnd] = deadEnd;
        eventBus->publish(AutoCity::EventType::TilesLoaded);
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
        //do we have a default tile?
        auto firstDefault = tiles.find(TileType::Default);
        if (firstDefault != tiles.end()){
            auto defaultSubMap = firstDefault->second;
            auto defaultTile = defaultSubMap.find(TileSubType::NoFlow);
            if (defaultTile != defaultSubMap.end()){
                return defaultTile->second;
            }
        }
        //create a default tile and add it
        sf::Vector2u defaultDimensions(32, 32);
        sf::Texture texture(defaultDimensions);
        sf::Image image(defaultDimensions, sf::Color::Black);
        texture.update(image);
        sf::Sprite sprite(texture);
        sf::Angle angle = sf::degrees(0);
        Tile defaultTile = {TileType::Default, TileSubType::NoFlow, sprite, {angle, angle, angle, angle}, angle};
        tiles[TileType::Default][TileSubType::NoFlow] = defaultTile;
        return defaultTile;
    };

    std::map<TileType, std::map<TileSubType, Tile>> TileManager::getTiles(){
        return tiles;
    }
};