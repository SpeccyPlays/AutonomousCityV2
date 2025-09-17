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
    struct Tile {
        TileType type;
        TileSubType subType;
        sf::Sprite sprite;
        std::vector<sf::Angle> flowMap;
        sf::Angle rotation;
    };
};