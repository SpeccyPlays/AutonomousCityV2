#include "EventBus.h"

namespace AutoCity {
    
    void EventBus::publish(Event& event){
        for (auto& callback : subscribers[event.type]) {
            callback(event);
        }
    };
    void EventBus::publish(EventType type){
        //empty payload as not always required
        Event event = { type, {} };
        
        publish(event);
    };
    void EventBus::subscribe(EventType type, std::function<void(const Event&)> callback){
        subscribers[type].push_back(callback);
    };

};