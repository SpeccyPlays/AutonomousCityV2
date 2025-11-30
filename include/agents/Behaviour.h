#pragma once

#include "../include/agents/PerceptionData.h"

namespace AutoCity {
    class Behaviour {
        public :
            struct Behaviours {
                bool braking;
                bool steering;
                bool accelerate;
                float steeringAmount;
                float brakingMultiplier;
                float accelerationAmount;
            };
            virtual Behaviours decideActions(const PerceptionData& perceptionData) = 0;
            bool offGridCheck(const  std::array<bool, 4>& offGrid);
            void offGridHandler(Behaviours* behaviour, const std::array<bool, 4>& offGrid, sf::Vector2f velocity);
            bool isNotRoadTile(const PerceptionData& perceptionData);
            bool isNotAlone(const PerceptionData& perceptionData);
            void tileActions(Behaviours* behaviour, const PerceptionData& perceptionData);
            float wrapAngle(float angle);
            virtual ~Behaviour() = default;
    };
    class NormalDriver : public Behaviour {
        public:
            Behaviours decideActions(const PerceptionData& perceptionData) override;
    };
    class OldPerson : public Behaviour {
        public:
            Behaviours decideActions(const PerceptionData& perceptionData) override;
    };
    class BoyRacer : public Behaviour {
        public:
            Behaviours decideActions(const PerceptionData& perceptionData) override;
    };
};