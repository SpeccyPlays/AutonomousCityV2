#pragma once

#include "../include/objects/CityObject.h"
#include <random>

namespace AutoCity {
    class Agent : public CityObject{
        protected:
            enum class AgentState {
                Standard,
                Wandering
            };
            sf::Vector2f currentPos;
            sf::Vector2f velocity;
            sf::Vector2f acceleration;
            sf::Vector2f desiredPos;
            sf::Vector2f lookAheadPos;
            float currentDeltaTime;
            float angle;
            float maxspeed;
            float currentSpeed;
            float accelerationRate;
            float decelerationRate;
            float wanderingDistance;
            AgentState agentState;
            std::string texturePath;
            //used for random wandering
            std::mt19937 rngSeed;
            std::uniform_real_distribution<float> wanderDist;
            std::uniform_real_distribution<float> randomAngle;
        private:
            
            void wrapAngle();
        public:
            Agent(sf::RenderWindow& window, AutoCity::EventBus& bus);
            void init() override;
            void processEvents(const sf::Event& event) override;
            void update(sf::Time delta) override;
            void draw() override;
            sf::Vector2f getPos();
            sf::Vector2f getLookAheadPos();
            sf::Vector2f getDesiredPos(sf::Time delta);
            sf::Vector2f getnextPos();
            sf::Vector2f getVelocity();
            void slowDown();
            void setDesired();
            void setVelocity();
            void setCurrentPosToDesired();
            void steerLeft();
            void steerRight();
            void accelerate();
        };
};