#include "../include/controllers/AgentController.h"
#include <iostream>

namespace AutoCity {
    AgentController::AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){

    };
    void AgentController::init(){
        bus.subscribe(AutoCity::EventType::DebugAgents, [this](const Event& e) { this->toggleDebug(); });
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

    };
    void AgentController::collisionHandler(const Event& e){

    };
    void AgentController::roadFlowHandler(const Event& e){

    }
};