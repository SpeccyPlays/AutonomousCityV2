#include "../include/agents/Behaviour.h"

namespace AutoCity {
    Behaviour::Behaviours NormalDriver::decideActions(const PerceptionData &perceptionData){
        Behaviours actions{};
        return actions;
    };
    Behaviour::Behaviours BoyRacer::decideActions(const PerceptionData &perceptionData){
        Behaviours actions;
        return actions;
    };
    Behaviour::Behaviours OldPerson::decideActions(const PerceptionData &perceptionData){
        Behaviours actions;
        return actions;
    };
}