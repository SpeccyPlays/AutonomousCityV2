#pragma once

#include <SFML/Graphics.hpp>

namespace AutoCity {
    
    enum class TileType {
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
        sf::IntRect spriteSheetPos;
        std::vector<sf::Angle> flowMap;
        sf::Angle rotation;
    };
};