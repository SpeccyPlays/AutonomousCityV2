#include "../include/CityGrid/TileManager.h"

namespace AutoCity {
    
    std::map<TileType, std::map<TileSubType, Tile>> TileManager::tiles;
    EventBus* TileManager::eventBus = nullptr;
    sf::Vector2i TileManager::tileSize = {32, 32};
    
    void TileManager::setEventBus(EventBus& bus){
        eventBus = &bus;
    }
    void TileManager::setBasicTiles(AutoCity::TextureManager& texManager){
        //create a default tile and add it
        sf::Vector2u defaultDimensions(tileSize);
        sf::Texture defaultTexture(defaultDimensions);
        sf::Image image(defaultDimensions, sf::Color::Black);
        defaultTexture.update(image);
        sf::Sprite sprite(defaultTexture);
        sf::Vector2f origin = {tileSize.x / 2.f, tileSize.y / 2.f};
        sf::Angle angle = sf::degrees(0);
        Tile defaultTile = {TileType::Default, TileSubType::NoFlow, sprite, origin, {angle, angle, angle, angle}, angle};
        tiles[TileType::Default].emplace(TileSubType::NoFlow, defaultTile);
        
        //sf::Texture& texture = tex;
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
        sf::Texture& roadStraightTex = texManager.getTexture("include/textures/roadstraight.png");
        sf::Sprite roadStraightSprite(roadStraightTex);
        Tile roadStraight = {
            TileType::Road, 
            TileSubType::Straight, 
            roadStraightSprite,
            origin, 
            {north, south, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road].emplace(TileSubType::Straight, roadStraight); //add like this or it goes boom

        sf::Texture& leftTurnTex = texManager.getTexture("include/textures/leftturn.png");
        sf::Sprite leftTurnSprite(leftTurnTex);
        Tile leftTurn = {
            TileType::Road, 
            TileSubType::LeftTurn, 
            leftTurnSprite,
            origin,
            {east, south, south, west}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road].emplace(TileSubType::LeftTurn, leftTurn);

        sf::Texture& rightTurnTex = texManager.getTexture("include/textures/rightturn.png");
        sf::Sprite rightTurnSprite(rightTurnTex);
        Tile rightTurn = {
            TileType::Road, 
            TileSubType::RightTurn, 
            rightTurnSprite,
            origin,
            {east, east, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road].emplace(TileSubType::RightTurn, rightTurn);

        sf::Texture& tJuncTex = texManager.getTexture("include/textures/tjunc.png");
        sf::Sprite tJuncSprite(tJuncTex);
        Tile tJunction = {
            TileType::Road, 
            TileSubType::TJunction, 
            tJuncSprite,
            origin,
            {east, east, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road].emplace(TileSubType::TJunction, tJunction);

        sf::Texture& deadEndTex = texManager.getTexture("include/textures/deadend.png");
        sf::Sprite deadEndSprite(deadEndTex);
        Tile deadEnd = {
            TileType::Road, 
            TileSubType::DeadEnd, 
            deadEndSprite,
            origin,
            {east, south, south, north}, //flowmap
            sf::degrees(0)
        };
        tiles[TileType::Road].emplace(TileSubType::DeadEnd, deadEnd);
        Event event = {AutoCity::EventType::TilesLoaded, tiles};
        eventBus->publish(event);
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
        //create a default tile and add it - shouldn't get to this but just in case
        sf::Vector2u defaultDimensions(tileSize);
        sf::Texture texture(defaultDimensions);
        sf::Image image(defaultDimensions, sf::Color::Black);
        texture.update(image);
        sf::Sprite sprite(texture);
        sf::Vector2f origin = {tileSize.x / 2.f, tileSize.y / 2.f};
        sf::Angle angle = sf::degrees(0);
        Tile defaultTile = {TileType::Default, TileSubType::NoFlow, sprite, origin, {angle, angle, angle, angle}, angle};
        tiles[TileType::Default].emplace(TileSubType::NoFlow, defaultTile);
        return defaultTile;
    };

    std::map<TileType, std::map<TileSubType, Tile>>& TileManager::getTiles(){
        return tiles;
    }
};