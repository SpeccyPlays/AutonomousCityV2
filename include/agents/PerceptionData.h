#pragma once
#include "../CityGrid/Tile.h"
#include <unordered_set>
namespace AutoCity {
    class Agent;
    struct PerceptionData {
        sf::Vector2f velocity;
        std::array<bool, 4> boundaryOffGrid;
        std::array<bool, 4> desiredOffGrid;
        TileType tileType;
        TileSubType tileSubType;
        float tileFlowAngle;
        float agentAngle;
        std::unordered_set<AutoCity::Agent*> occupants;
    };
}