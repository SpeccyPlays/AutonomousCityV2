#include "../include/agents/Agent.h"
#include "../include/textures/TextureManager.h"

namespace AutoCity {
    Agent::Agent(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){
        sf::Vector2f windowSize = static_cast<sf::Vector2f>(window.getSize());
        currentPos.x = windowSize.x / 2;
        currentPos.y = windowSize.y / 2;
    };
    void Agent::init(){
        maxspeed = 50;
        wanderingDistance = 0.01f;
        agentState = AgentState::Wandering;
        velocity = {0.f, 0.f};
        accelerationRate = maxspeed * 0.1f;
        decelerationRate = 0.8f;
        currentSpeed = 0.f;
        rngSeed = std::mt19937(std::random_device{}());
        wanderDist = std::uniform_real_distribution<float>(-wanderingDistance, wanderingDistance);
        std::uniform_real_distribution<float> angleDist(-1.5708f, 1.5708f);
        angle = angleDist(rngSeed);
        texturePath = "include/textures/car.png";
        AutoCity::TextureManager::getTexture("include/textures/car.png");
        AutoCity::TextureManager::getTexture("include/textures/carboyracer.png");
        AutoCity::TextureManager::getTexture("include/textures/caroldperson.png");
    };
    void Agent::processEvents(const sf::Event& event){

    };
    void Agent::update(sf::Time delta){

    };
    void Agent::draw(){
        sf::Texture tex = AutoCity::TextureManager::getTexture(texturePath);
        sf::Sprite sprite(tex);
        sprite.setPosition(currentPos);
        window.draw(sprite);
    };
    sf::Vector2f Agent::getPos(){
        return currentPos;
    };
    void Agent::offTopOfGrid(){

    };
    void Agent::offBottomOfGrid(){

    };
    void Agent::offLeftOfGrid(){

    };
    void Agent::offRightOfGrid(){
        
    };  
};