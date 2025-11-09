#include "../include/controllers/AgentController.h"
#include <iostream>
#include <unordered_set>
#include <array>
#include <algorithm>

namespace AutoCity {
    AgentController::AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){
        for (int i = 0; i < 1; i++){
            std::unique_ptr<AutoCity::Agent> agent;
            agent = std::make_unique<AutoCity::Agent>(window, bus);
            agents.emplace_back(std::move(agent));
        }
    };
    void AgentController::init(){
        bus.subscribe(AutoCity::EventType::DebugAgents, [this](const Event& e) { this->toggleAllDebug(); });
        bus.subscribe(AutoCity::EventType::LookAheadResponse, [this](const Event& e) { this->handleLookAheadBoundryCheck(e); });
        bus.subscribe(AutoCity::EventType::DesiredBoundaryCheckResponse, [this](const Event& e) { this->handleDesiredBoundryCheck(e); });
        bus.subscribe(AutoCity::EventType::AgentCollisionCheckResponse, [this](const Event& e) { this->collisionHandler(e); });
        bus.subscribe(AutoCity::EventType::AgentTile, [this](const Event& e) { this->tileHandler(e); });
        
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.init();
        }
    };
    void AgentController::processEvents(const sf::Event& event){
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.processEvents(event);
        }
    };
    void AgentController::update(sf::Time delta){
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.update(delta); //just sets currentDeltaTime in agent
            //remove agent from current cell
            Event removeEvent = {EventType::RemoveAgent, std::pair{&agent, agent.getPos()}};
            bus.publish(removeEvent);
            Event flowMapCheck = {EventType::AgentGetTile, std::pair{&agent, agent.getPos()}};
            bus.publish(flowMapCheck);
            //set off grid to false by default
            agent.setOffGrid(false);
            //Check further ahead to see if agent heading offgrid
            agent.accelerate();
            Event lookAheadEvent = {EventType::AgentLookAheadBoundaryCheck, std::pair{&agent, agent.getLookAheadPos()}};
            bus.publish(lookAheadEvent);
            Event desiredEvent = {EventType::AgentDesiredBoundaryCheck, std::pair{&agent, agent.getDesiredPos(delta)}};
            bus.publish(desiredEvent);
            if (!agent.getOffGrid()){
                Event collisionCheckEvent = {EventType::AgentCollisionCheck, std::pair{&agent, agent.getPos()}};
                bus.publish(collisionCheckEvent);
            };            
            agent.setVelocity();
            agent.setDesired();
            Event upDateEvent = {EventType::AgentUpdate, std::pair{&agent, agent.getnextPos()}};
            bus.publish(upDateEvent);
            agent.setCurrentPosToDesired();
        };
    };
    void AgentController::draw(){
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.draw();
        }
    };
    void AgentController::handleLookAheadBoundryCheck(const Event& e){
        const auto& payload = std::any_cast<std::pair<Agent*, std::array<bool, 4>>>(e.payload);
        Agent* agent = payload.first;
        const std::array<bool, 4>& offGrid = payload.second;
        int offCount = std::count(offGrid.begin(), offGrid.end(), true);
        if (offCount > 0){
            offGridHandler(agent, offGrid);
        };
    };
    void AgentController::handleDesiredBoundryCheck(const Event& e){
        const auto& payload = std::any_cast<std::pair<Agent*, std::array<bool, 4>>>(e.payload);
        Agent* agent = payload.first;
        const std::array<bool, 4>& offGrid = payload.second;
        int offCount = std::count(offGrid.begin(), offGrid.end(), true);
        if (offCount > 0){
            agent->setOffGrid(true);
            offGridHandler(agent, offGrid);
        }
    };
    void AgentController::offGridHandler(Agent* agent, std::array<bool, 4> offGrid){
        //offGrid array is Top, Right, Bottom, Left
        //For the off grids, check velocities to work out which way to steer
        //a negative x means agent going right to left, oppposite if positive
        //a negative y means agents going bottom to top, oppposite if positive
        sf::Vector2f velocity = agent->getVelocity();
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
        agent->slowDown();
        agent->addSteering(steeringAmount);
    };
    void AgentController::collisionHandler(const Event& e){
        //payload std::pair<Agent*, std::unordered_set<AutoCity::Agent *>
        const auto& payload = std::any_cast<std::pair<Agent*, std::unordered_set<AutoCity::Agent*>>>(e.payload);
        Agent* agent = payload.first;
        const std::unordered_set<AutoCity::Agent*>& occupants = payload.second;
    };
    void AgentController::tileHandler(const Event& e){
        //payload std::pair<Agent*, std::vector<sf::Angle>
        const auto& payload = std::any_cast<std::pair<Agent*, Tile>>(e.payload);
        Agent* agent = payload.first;
        Tile tile = payload.second;
        //work out where we are in the tile
        sf::Vector2f agentNextPos = agent->getnextPos();
        if (tile.type == TileType::Default){
            agent->slowDown();
            agent->addSteering(2.f);
        }
        else {
            float tileFlowAngle = getTileFlowAngle(tile, agentNextPos);
            tileAngleActions(agent, tileFlowAngle);
        };   
    };
    void AgentController::toggleAllDebug(){
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.toggleDebug();
        }
    };
    float AgentController::getTileFlowAngle(Tile tile, sf::Vector2f pos){
        float xPos = fmod(pos.x, AutoCity::TileManager::tileSize.x);
        float yPos = fmod(pos.y, AutoCity::TileManager::tileSize.y);
        float flowAngle = 0;
        //get the remainder to find out cellPos
        int cellPos = 0;
        if (xPos < AutoCity::TileManager::tileSize.x / 2 && yPos < AutoCity::TileManager::tileSize.y / 2){
            //Top left, first in flowmap
            flowAngle = tile.flowMap[0].asDegrees();
        }
        else if (xPos > AutoCity::TileManager::tileSize.x / 2 && yPos < AutoCity::TileManager::tileSize.y / 2){
            //top right, second in flowmap
            cellPos = 1;
            flowAngle = tile.flowMap[1].asDegrees();
        }
        else if (xPos > AutoCity::TileManager::tileSize.x / 2 && yPos > AutoCity::TileManager::tileSize.y / 2){
            //bottom right, third in flowmap
            cellPos = 2;
            flowAngle = tile.flowMap[2].asDegrees();
        }
        else if (xPos < AutoCity::TileManager::tileSize.x / 2 && yPos > AutoCity::TileManager::tileSize.y / 2){
            //bottom left, fouth in flowmap
            cellPos = 3;
            flowAngle = tile.flowMap[3].asDegrees();
        };
        return flowAngle;
    };
    void AgentController::tileAngleActions(Agent* agent, float tileAngle){
        //chat gpt helped with this part
        //I don't fully understand it
        float allowedDifference = 5.f;
        float agentAngle = agent->getAngle();
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
            agent->slowDown();
        }
        else if (std::abs(angleDiff) > 45.f) {
            agent->slowDown();
        }
        std::cout << "Agent angle: " << agentAngle << " Tile flow angle: " << tileAngle << " Angle diff: "<< angleDiff << " Steering amount:" << steeringAmount << std::endl;
        // Apply steering smoothly
        agent->addSteering(steeringAmount);
        };
    float AgentController::wrapAngle(float angle){
        //wraps to be in -180 to 180 range
        if (angle < -180) {
            angle += 360.0f;
        };
        if (angle > 180){
            angle -= 360.0f;
        }
        return angle;
    };
};