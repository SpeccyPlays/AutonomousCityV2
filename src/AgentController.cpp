#include "../include/controllers/AgentController.h"
#include <iostream>

namespace AutoCity {
    AgentController::AgentController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){

    };
    void AgentController::init(){
        bus.subscribe(AutoCity::EventType::AgentAdded, [this](const Event& e) { agentAdded(e); });
        bus.subscribe(AutoCity::EventType::DebugAgents, [this](const Event& e) { this->toggleDebug(); });
    };
    void AgentController::processEvents(const sf::Event& event){

    };
    void AgentController::update(sf::Time delta){

    };
    void AgentController::draw(){
        
    };
    void AgentController::agentAdded(const Event& e){
        if (this->getDebug()){
            const auto& ev = std::any_cast<std::pair<Agent*, sf::Vector2u>>(e.payload);
            const auto& [agent, pos] = ev;
            std::cout << "Agent added at X: " << pos.x << " Y:" << pos.y << std::endl;
        };
    };
};