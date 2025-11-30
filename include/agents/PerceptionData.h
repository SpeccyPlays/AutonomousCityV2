#pragma once
#include "../CityGrid/Tile.h"

namespace AutoCity {
    struct PerceptionData {
        sf::Vector2f velocity;
        sf::Vector2f currentPos;
        sf::Vector2f textureSize;
        float currentSpeed;
        std::array<bool, 4> boundaryOffGrid;
        std::array<bool, 4> desiredOffGrid;
        TileType tileType;
        TileSubType tileSubType;
        float tileFlowAngle;
        float agentAngle;
        //std::unordered_set<AutoCity::Agent*> occupants;
        std::vector<sf::Vector2f> occupantPositions;
    };
};