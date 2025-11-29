#pragma once

#include "../include/objects/CityObject.h"
#include "../CityGrid/Tile.h"
#include "../include/agents/Behaviour.h"
#include "../include/agents/PerceptionData.h"
#include <random>
#include <unordered_set>

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
            AgentState agentState;
            std::string texturePath;
            //used for random wandering
            std::mt19937 rngSeed;
            std::uniform_real_distribution<float> randomAngle;
            bool offGrid;
        private:
            void wrapAngle();
        public:
            PerceptionData perceptionData;
            std::unique_ptr<Behaviour> behaviour;
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
            float getSpeed();
            float getAngle();
            void slowDown();
            void slowDownBy(float reductionMulipier);
            void setDesired();
            void setVelocity();
            void setCurrentPosToDesired();
            void accelerate();
            void addAcceleration(float accelerationAmount);
            void setOffGrid(bool isOffGrid);
            bool getOffGrid();
            void addSteering(float steeringAmount);
            AgentState getState();
            std::string getTexturePath();
            //used for loading only
            void setCurrentPos(sf::Vector2f pos);
            void setVelocity(sf::Vector2f newVelocity);
            void setTexturePath(std::string path);
            void setAngle(float newAngle);
            void setSpeed(float newSpeed);
            void action(Behaviour::Behaviours behaviours);
        };
};