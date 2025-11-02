#include "../include/controllers/AgentController.h"
#include <iostream>
#include <unordered_set>
#include <array>
#include <algorithm>

namespace AutoCity {
    AgentController::AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){
        std::unique_ptr<AutoCity::Agent> agent;
        agent = std::make_unique<AutoCity::Agent>(window, bus);
        agents.emplace_back(std::move(agent));
    };
    void AgentController::init(){
        bus.subscribe(AutoCity::EventType::DebugAgents, [this](const Event& e) { this->toggleAllDebug(); });
        bus.subscribe(AutoCity::EventType::LookAheadResponse, [this](const Event& e) { this->handleLookAheadBoundryCheck(e); });
        bus.subscribe(AutoCity::EventType::DesiredBoundaryCheckResponse, [this](const Event& e) { this->handleDesiredBoundryCheck(e); });
        bus.subscribe(AutoCity::EventType::AgentCollision, [this](const Event& e) { this->collisionHandler(e); });
        bus.subscribe(AutoCity::EventType::RoadFlowMap, [this](const Event& e) { this->roadFlowHandler(e); });
        
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
            //remove agent from current cell
            Event removeEvent = {EventType::RemoveAgent, std::pair{&agent, agent.getPos()}};
            bus.publish(removeEvent);
            //Check further ahead to see if agent heading offgrid
            agent.accelerate();
            Event lookAheadEvent = {EventType::AgentLookAheadBoundaryCheck, std::pair{&agent, agent.getLookAheadPos()}};
            bus.publish(lookAheadEvent);
            Event desiredEvent = {EventType::AgentDesiredBoundaryCheck, std::pair{&agent, agent.getDesiredPos(delta)}};
            agent.setVelocity();
            agent.setDesired();
            bus.publish(desiredEvent); 
            Event event = {EventType::AgentUpdate, std::pair{&agent, agent.getnextPos()}};
            bus.publish(event);
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
            agent->slowDown();
            offGridHandler(agent, offGrid);
        }
    };
    void AgentController::offGridHandler(Agent* agent, std::array<bool, 4> offGrid){
        //offGrid array is Top, Right, Bottom, Left
        //For the off grids, check velocities to work out which way to steer
        //a negative x means agent going right to left, oppposite if positive
        //a negative y means agents going bottom to top, oppposite if positive
        sf::Vector2f velocity = agent->getVelocity();
        if (offGrid[0] == true){
            if (velocity.x < 0){
                agent->steerLeft();
            }
            else {
                agent->steerRight();
            };
        }
        else if (offGrid[2] == true){
            if (velocity.x < 0){
                agent->steerRight();
            }
            else {
                agent->steerLeft();
                
            };
        };
        //either off right or left, not both
        if (offGrid[1] == true){
            if (velocity.y < 0){
                agent->steerLeft();
            }
            else {
                agent->steerRight();
            };
        }
        else if (offGrid[3] == true){
            if (velocity.y < 0){
                agent->steerRight(); 
            }
            else {
                agent->steerLeft();
            };
        };
    };
    void AgentController::collisionHandler(const Event& e){
        //payload std::pair<Agent*, std::unordered_set<AutoCity::Agent *>
        const auto& payload = std::any_cast<std::pair<Agent*, std::unordered_set<AutoCity::Agent*>>>(e.payload);
        Agent* agent = payload.first;
        const std::unordered_set<AutoCity::Agent*>& occupants = payload.second;
    };
    void AgentController::roadFlowHandler(const Event& e){
        //payload std::pair<Agent*, std::vector<sf::Angle>
        const auto& payload = std::any_cast<std::pair<Agent*, std::vector<sf::Angle>>>(e.payload);
        Agent* agent = payload.first;
        std::vector<sf::Angle> flowMap = payload.second;
        agent->setCurrentPosToDesired();
    };
    void AgentController::toggleAllDebug(){
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.toggleDebug();
        }
    };
};