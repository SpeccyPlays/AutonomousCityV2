#include "../include/agents/Agent.h"
#include "../include/textures/TextureManager.h"
#include <iostream>
#include <cmath>
#define M_PI 3.14159265358979323846  /* pi */

namespace AutoCity {
    Agent::Agent(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){
        sf::Vector2f windowSize = static_cast<sf::Vector2f>(window.getSize());
        currentPos.x = windowSize.x / 2;
        currentPos.y = windowSize.y / 2;
    };
    void Agent::init(){
        maxspeed = 1.5f;
        velocity = {0.f, 0.f};
        accelerationRate = 5.f;//random value but never mind
        decelerationRate = 0.95f;
        currentSpeed = 0.f;
        rngSeed = std::mt19937(std::random_device{}());
        std::uniform_real_distribution<float> angleDist(0, 360);
        angle = angleDist(rngSeed);
        texturePath = "include/textures/car.png";
        offGrid = false;
        behaviour = std::make_unique<NormalDriver>();
    };
    void Agent::processEvents(const sf::Event& event){

    };
    void Agent::update(sf::Time delta){
        currentDeltaTime = delta.asSeconds();
        perceptionData.velocity = velocity;
    };
    void Agent::draw(){
        sf::Texture &tex = AutoCity::TextureManager::getTexture(texturePath);
        sf::Sprite sprite(tex);
        sf::Vector2f texSize = static_cast<sf::Vector2f>(tex.getSize());
        sprite.setOrigin({texSize.x / 2.f, texSize.y / 2.f});
        sprite.setPosition(currentPos);
        sprite.setRotation(sf::degrees(angle));
        window.draw(sprite);
        if (CityObject::getDebug()){
            std::array line = {
                sf::Vertex{currentPos},
                sf::Vertex{lookAheadPos}
            };
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        };        
    };
    sf::Vector2f Agent::getPos(){
        return currentPos;
    };
    sf::Vector2f Agent::getLookAheadPos(){
        float speed = currentSpeed;
        speed += accelerationRate * 10;
        sf::Vector2f tempVelocity({0, 0});
        double radians = angle * M_PI / 180.0;
        tempVelocity.x = std::cos(radians) * speed;
        tempVelocity.y = std::sin(radians) * speed;
        sf::Vector2f tempLookAheadPos = currentPos + tempVelocity;
        lookAheadPos = tempLookAheadPos;
        return lookAheadPos;
    };
    sf::Vector2f Agent::getDesiredPos(sf::Time delta){
        currentDeltaTime = delta.asSeconds();
        float speed = currentSpeed;
        speed += accelerationRate * currentDeltaTime;
        if (speed > maxspeed){
            speed = maxspeed;
        };
        sf::Vector2f tempVelocity({0, 0});
        double radians = angle * M_PI / 180.0;
        tempVelocity.x = std::cos(radians) * speed;
        tempVelocity.y = std::sin(radians) * speed;
        sf::Vector2f tempDesiredPos = currentPos + tempVelocity;
        return tempDesiredPos;
    };
    sf::Vector2f Agent::getnextPos(){
        return desiredPos;
    }
    sf::Vector2f Agent::getVelocity(){
        return velocity;
    };
    float Agent::getAngle(){
        return angle;
    };
    void Agent::addSteering(float steeringAmount){
        angle += steeringAmount;
        wrapAngle();
    };
    void Agent::accelerate(){
        //copied from previous version
        currentSpeed += accelerationRate * currentDeltaTime;
        if (currentSpeed > maxspeed){
            currentSpeed = maxspeed;
        };
    };
    void Agent::addAcceleration(float accelerationAmount){
        currentSpeed += accelerationAmount * currentDeltaTime;
    };
    void Agent::slowDown(){
        //copied from previous version
        currentSpeed *= decelerationRate;
        if (currentSpeed < 0.2f){
            currentSpeed = 0.2f;
        }
    };
    void Agent::slowDownBy(float reductionMulipier){
        currentSpeed *= reductionMulipier;
        //stops agents getting completely stuck
        if (currentSpeed < 0.2f){
            currentSpeed = 0.2f;
        }
    };
    void Agent::setVelocity(){
        //copied from previous version
        double radians = angle * M_PI / 180.0;
        velocity.x = std::cos(radians) * currentSpeed;
        velocity.y = std::sin(radians) * currentSpeed;
    };
    void Agent::setDesired(){
        //copied from previous version
        desiredPos = currentPos + velocity;// * currentDeltaTime;
    };
    void Agent::setCurrentPosToDesired(){
        currentPos = desiredPos;
    };
    void Agent::wrapAngle(){
        //Wrap or the agent ends up with some mad angles.
        angle = std::fmod(angle, 360.0f); // get remainder after division by 360
        if (angle < 0) {
            angle += 360.0f; // ensure it's positive
        }
    };
    void Agent::setOffGrid(bool isOffGrid){
        offGrid = isOffGrid;
    };
    bool Agent::getOffGrid(){
        return offGrid;
    };
    Agent::AgentState Agent::getState(){
        return agentState;
    };
    std::string Agent::getTexturePath(){
        return texturePath;
    };
    float Agent::getSpeed(){
        return currentSpeed;
    }
    //used for loading only
    void Agent::setCurrentPos(sf::Vector2f pos){
        currentPos = pos;
    };
    void Agent::setVelocity(sf::Vector2f newVelocity){
        velocity = newVelocity;
    };
    void Agent::setTexturePath(std::string path){
        texturePath = path;
    };
    void Agent::setAngle(float newAngle){
        angle = newAngle;
    };
    void Agent::setSpeed(float newSpeed){
        currentSpeed = newSpeed;
    };
    Behaviour::Behaviours Agent::decideActions(const PerceptionData &perceptionData){
        return behaviour->decideActions(perceptionData);
    };
    void Agent::action(Behaviour::Behaviours behaviours){
        if (behaviours.braking){
            slowDownBy(behaviours.brakingMultiplier);
        };
        if (behaviours.accelerate){
            addAcceleration(behaviours.accelerationAmount);
        };
        if (behaviours.steering){
            addSteering(behaviours.steeringAmount);
        };
        setVelocity();
        setDesired();
    };
};