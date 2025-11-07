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
        wanderingDistance = 0.01f;
        agentState = AgentState::Wandering;
        velocity = {0.f, 0.f};
        accelerationRate = 5.f;//random value but never mind
        decelerationRate = 0.8f;
        currentSpeed = 0.f;
        rngSeed = std::mt19937(std::random_device{}());
        wanderDist = std::uniform_real_distribution<float>(-wanderingDistance, wanderingDistance);
        std::uniform_real_distribution<float> angleDist(0, 360);
        angle = angleDist(rngSeed);
        texturePath = "include/textures/car.png";
        offGrid = false;
        /*AutoCity::TextureManager::getTexture("include/textures/car.png");
        AutoCity::TextureManager::getTexture("include/textures/carboyracer.png");
        AutoCity::TextureManager::getTexture("include/textures/caroldperson.png");*/
    };
    void Agent::processEvents(const sf::Event& event){

    };
    void Agent::update(sf::Time delta){
        currentDeltaTime = delta.asSeconds();
    };
    void Agent::draw(){
        sf::Texture tex = AutoCity::TextureManager::getTexture(texturePath);
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
    void Agent::steerLeft(){
        //copied from previous version
        float steeringAmount = 5;
        angle -= steeringAmount;
        wrapAngle();
    };
    void Agent::steerRight(){
        //copied from previous version
        float steeringAmount = 5;
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
    void Agent::slowDown(){
        //copied from previous version
        currentSpeed *= decelerationRate;
        if (currentSpeed < 1){
            currentSpeed = 0.f;
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
    }
};