#define _USE_MATH_DEFINES
#include "../include/agents/Behaviour.h"
#include <algorithm>
#include <math.h>
#include <iostream>

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
            //behaviour->braking = true;
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
        if (perceptionData.occupantPositions.size() > 1){
            return true;
        }
        return false;
    };
    Behaviour::Sectors Behaviour::checkAroundAgent(const PerceptionData& perceptionData){
        Sectors sectors = {};
        sf::Vector2f currentPos = perceptionData.currentPos;
        float currentAngle = perceptionData.agentAngle;
        float seeingDistance = perceptionData.textureSize.x * 2; //x will be the agents length so look two agent lengths in front
        //Another chatgpt specials
        //check in sectors around the agent to see if other agents are around
        for (auto occupantPos : perceptionData.occupantPositions){
            sf::Vector2f diff = occupantPos - currentPos;
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (distance > seeingDistance){
                continue;
            };
            float angleToOther = std::atan2(diff.y, diff.x) * 180.f / M_PI;
            float relativeAngle = angleToOther - currentAngle;
            if (relativeAngle < -180.f){
                relativeAngle += 360.f;
            }
            if (relativeAngle > 180.f){
                relativeAngle -= 360.f;
            }
            if (relativeAngle >= -30.f && relativeAngle < -7.5f){
                 sectors.frontLeft = true;
            }
            else if (relativeAngle >= -7.5f && relativeAngle <= 7.5f){
                sectors.frontCenter = true;
            } 
            else if (relativeAngle > 7.5f && relativeAngle <= 30.f){
                sectors.frontRight = true;
            }
            else if (relativeAngle >= 60.f && relativeAngle <= 120.f){
                sectors.left = true;
            }
            else if (relativeAngle >= -120.f && relativeAngle <= -60.f){
                sectors.right = true;
            }
        };
        return sectors;
    };
    Behaviour::Behaviours NormalDriver::decideActions(const PerceptionData &perceptionData){
        Behaviours actions{};

        if (isNotRoadTile(perceptionData)){
            actions.braking = true;
            actions.brakingMultiplier = 0.95f;
            actions.steering = true;
            actions.steeringAmount = 2.f;
        } else {
            actions.accelerate = true;
            actions.accelerationAmount = 5.f;
            tileActions(&actions, perceptionData);
        }
        if (offGridCheck(perceptionData.boundaryOffGrid)){
            offGridHandler(&actions, perceptionData.boundaryOffGrid, perceptionData.velocity);
            return actions;
        };
        if (offGridCheck(perceptionData.desiredOffGrid)){
            actions.braking = true;
            actions.brakingMultiplier = 0.95f;
            offGridHandler(&actions, perceptionData.desiredOffGrid, perceptionData.velocity);
            return actions;
        };
        if (isNotAlone(perceptionData)){
            Sectors sectors = checkAroundAgent(perceptionData);
            if (sectors.frontCenter || sectors.frontLeft || sectors.frontRight){
                actions.accelerate = false;
                actions.braking = true;
                actions.brakingMultiplier = 0.95f;
            }
            if (sectors.frontRight || sectors.right){
                actions.steering = true;
                actions.steeringAmount += 0.2f;
            }
            else if (sectors.frontLeft || sectors.left){
                actions.steering = true;
                actions.steeringAmount -= 0.2f;
            }
        }
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