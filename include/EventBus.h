#pragma once
#include <map>
#include <list>
#include <any>
#include <functional>

namespace AutoCity {
    enum class EventType {
        New,
        Open,
        Save,
        SaveAs,
        Quit,
        Debug,
        TilesLoaded,
        TileSelected,
        TileAdded,
        Error
    };
    struct Event {
        EventType type;
        std::any payload;
    };
    class EventBus {
        private: 
            std::map<EventType, std::list<std::function<void(const Event&)>>> subscribers;

        public:
            void subscribe(EventType type, std::function<void(const Event&)> callback);
            void publish(Event& event);
            void publish(EventType type);
    };
};