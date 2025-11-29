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