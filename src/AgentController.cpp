#include "../include/controllers/AgentController.h"
#include <iostream>
#include <unordered_set>
#include <array>

namespace AutoCity {
    AgentController::AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){
        std::unique_ptr<AutoCity::Agent> agent;
        agent = std::make_unique<AutoCity::Agent>(window, bus);
        agents.emplace_back(std::move(agent));
    };
    void AgentController::init(){
        bus.subscribe(AutoCity::EventType::DebugAgents, [this](const Event& e) { this->toggleAllDebug(); });
        bus.subscribe(AutoCity::EventType::AgentOffGrid, [this](const Event& e) { this->offGridHandler(e); });
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
            agent.update(delta);
            Event event = {EventType::AgentUpdate, std::pair{&agent, agent.getPos()}};
            bus.publish(event);
        };
    };
    void AgentController::draw(){
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.draw();
        }
    };
    void AgentController::offGridHandler(const Event& e){
        //payload std::pair<Agent*, std::array>
        const auto& payload = std::any_cast<std::pair<Agent*, std::array<bool, 4>>>(e.payload);
        Agent* agent = payload.first;
        const std::array<bool, 4>& offGrid = payload.second;
        //offGrid array is Top, Right, Bottom, Left
        //Either off Top or bottom, not both
        if (offGrid[0] == true){
            agent->offTopOfGrid();
        }
        else if (offGrid[2] == true){
            agent->offBottomOfGrid();
        };
        //either off right or left, not both
        if (offGrid[1] == true){
            agent->offRightOfGrid();
        }
        else if (offGrid[3] == true){
            agent->offLeftOfGrid();
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
    };
    void AgentController::toggleAllDebug(){
        for (auto& agentPtr : agents){
            Agent& agent = *agentPtr;
            agent.toggleDebug();
        }
    };
};