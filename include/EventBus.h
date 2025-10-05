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
        DebugGrid,
        DebugAgents,
        TilesLoaded, //payload std::map<TileType, std::map<TileSubType, Tile>>
        TileSelected, //payload &tile
        TileAdded, //payload of std::pair<Tile, sf::Vector2u> tileInfo
        AddAgent, //payload of std::pair<Agent*, sf::Vector2f>
        RemoveAgent, //payload of std::pair<Agent*, sf::Vector2f>
        AgentAdded, //payload of std::pair<Agent*, sf::Vector2u>
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