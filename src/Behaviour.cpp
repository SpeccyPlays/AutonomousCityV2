#include "../include/agents/Behaviour.h"
#include <algorithm>

namespace AutoCity {
    bool Behaviour::offGridCheck(const std::array<bool, 4>& offGrid){
        int offCount = std::count(offGrid.begin(), offGrid.end(), true);
        return offCount > 0;
    };
    void Behaviour::offGridHandler(Behaviours* behaviour, const std::array<bool, 4>& offGrid, sf::Vector2f velocity){
        //offGrid array is Top, Right, Bottom, Left
        //For the off grids, check velocities to work out which way to steer
        //a negative x means agent going right to left, oppposite if positive
        //a negative y means agents going bottom to top, oppposite if positive
        float steeringAmount = 0.f;
        float boundaryCorrection = 2.0f;
        if (offGrid[0] == true){
            if (velocity.x < 0){
                steeringAmount = -boundaryCorrection;
            }
            else {
                steeringAmount = boundaryCorrection;
            };
        }
        else if (offGrid[2] == true){
            if (velocity.x < 0){
                steeringAmount = boundaryCorrection;
            }
            else {
                steeringAmount = -boundaryCorrection;
            };
        };
        //either off right or left, not both
        if (offGrid[1] == true){
            if (velocity.y < 0){
                steeringAmount = -boundaryCorrection;
            }
            else {
                steeringAmount = boundaryCorrection;
            };
        }
        else if (offGrid[3] == true){
            if (velocity.y < 0){
                steeringAmount = boundaryCorrection; 
            }
            else {
                steeringAmount = -boundaryCorrection;
            };
        };
        behaviour->steering = true;
        behaviour->steeringAmount = steeringAmount;
    };
    float Behaviour::wrapAngle(float angle){
        //wraps to be in -180 to 180 range
        if (angle < -180) {
            angle += 360.0f;
        };
        if (angle > 180){
            angle -= 360.0f;
        }
        return angle;
    };
    bool Behaviour::isNotRoadTile(const PerceptionData& perceptionData){
        if (perceptionData.tileType == TileType::Default){
            return true;
        }
        return false;
    };
    void Behaviour::tileActions(Behaviours* behaviour, const PerceptionData& perceptionData){
        //I don't fully understand it
        float allowedDifference = 5.f;
        float agentAngle = perceptionData.agentAngle;
        float tileAngle = perceptionData.tileFlowAngle;
        float angleDiff = wrapAngle(tileAngle - agentAngle);
        float amountToSteer = 0;        
        if (std::abs(angleDiff) < allowedDifference){
            return;
        }        
        // Scale steering strength based on how far off we are (smooth turning)
        float maxTurnRate = 3.0f; // how strongly the agent can turn per frame
        float steerStrength = std::clamp(angleDiff / 90.f, -1.f, 1.f); 
        float steeringAmount = steerStrength * maxTurnRate;

        //If we're off by over 120, probably on wrong side of the road so steer left
        if (std::abs(angleDiff) >= 120.f){
            steeringAmount = -1 * maxTurnRate;
            behaviour->braking = true;
        }
        else if (std::abs(angleDiff) > 45.f) {
            behaviour->braking = true;
            behaviour->brakingMultiplier = 0.95f;
        }
        // Apply steering smoothly
        behaviour->steering = true;
        behaviour->steeringAmount = steeringAmount;
    };
    bool Behaviour::isNotAlone(const PerceptionData &perceptionData){
        if (perceptionData.occupants.size() > 1){
            return true;
        }
        return false;
    }
    Behaviour::Behaviours NormalDriver::decideActions(const PerceptionData &perceptionData){
        Behaviours actions{};
        if (offGridCheck(perceptionData.boundaryOffGrid)){
            offGridHandler(&actions, perceptionData.boundaryOffGrid, perceptionData.velocity);
            return actions;
        }
        if (offGridCheck(perceptionData.desiredOffGrid)){
            actions.braking = true;
            actions.brakingMultiplier = 0.95f;
            offGridHandler(&actions, perceptionData.desiredOffGrid, perceptionData.velocity);
            return actions;
        }
        if (isNotRoadTile(perceptionData)){
            actions.braking = true;
            actions.brakingMultiplier = 0.95f;
            actions.steering = true;
            actions.steeringAmount = 2.f;
            return actions;
        }
        if (isNotAlone(perceptionData)){
            
        }
        tileActions(&actions, perceptionData);
        actions.accelerate = true;
        actions.accelerationAmount = 5.f;
        return actions;
    };
    Behaviour::Behaviours BoyRacer::decideActions(const PerceptionData &perceptionData){
        Behaviours actions{};
        return actions;
    };
    Behaviour::Behaviours OldPerson::decideActions(const PerceptionData &perceptionData){
        Behaviours actions{};
        return actions;
    };
}