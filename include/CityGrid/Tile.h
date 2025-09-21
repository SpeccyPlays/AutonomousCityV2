#pragma once

#include <SFML/Graphics.hpp>

namespace AutoCity {
      
    enum class TileType {
        Default,//will be created in the tilemanager
        Road,
        Pavement,
        Grass,
        Building
    };
    enum class TileSubType {
        NoFlow,
        Straight,
        LeftTurn,
        RightTurn,
        DeadEnd,
        TJunction
    };
    //Makes our life easier to be able to string these
    inline const char* toString(TileType type) {
        switch (type) {
            case TileType::Default:  return "Default";
            case TileType::Road:     return "Road";
            case TileType::Pavement: return "Pavement";
            case TileType::Grass:    return "Grass";
            case TileType::Building: return "Building";
        }
        return "Unknown";
    };

    inline const char* toString(TileSubType subType) {
        switch (subType) {
            case TileSubType::NoFlow:    return "NoFlow";
            case TileSubType::Straight:  return "Straight";
            case TileSubType::LeftTurn:  return "LeftTurn";
            case TileSubType::RightTurn: return "RightTurn";
            case TileSubType::DeadEnd:   return "DeadEnd";
            case TileSubType::TJunction: return "TJunction";
        }
        return "Unknown";
    };

    struct Tile {
        TileType type;
        TileSubType subType;
        sf::Sprite sprite;
        sf::Vector2f origin;
        std::vector<sf::Angle> flowMap;
        sf::Angle rotation;
        std::string toString() const {
            return std::string(AutoCity::toString(type)) + "_" +
                   std::string(AutoCity::toString(subType)) + "_" + std::to_string(static_cast<int>(rotation.asDegrees()));
        }
    };

    
};