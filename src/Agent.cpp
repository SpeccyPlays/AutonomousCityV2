#include "../include/agents/Agent.h"
#include "../include/textures/TextureManager.h"

namespace AutoCity {
    Agent::Agent(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){

    };
    void Agent::init(){
        AutoCity::TextureManager::getTexture("include/textures/car.png");
        AutoCity::TextureManager::getTexture("include/textures/carboyracer.png");
        AutoCity::TextureManager::getTexture("include/textures/caroldperson.png");
    };
    void Agent::processEvents(const sf::Event& event){

    };
    void Agent::update(sf::Time delta){

    };
    void Agent::draw(){

    };
};