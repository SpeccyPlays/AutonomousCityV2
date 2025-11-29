#pragma once
#include "../CityGrid/Tile.h"
#include <unordered_set>
namespace AutoCity {
    class Agent;
    struct PerceptionData {
        std::array<bool, 4> boundaryOffGrid;
        std::array<bool, 4> desiredOffGrid;
        TileType tileType;
        TileSubType tileSubType;
        float tileFlowAngle;
        std::unordered_set<AutoCity::Agent*> occupants;
    };
}