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
        TileAdded, //payload of std::pair<Tile, sf::Vector2i> tileInfo
        RemoveAgent, //std::pair<Agent*, sf::Vector2f>
        AgentLookAheadBoundaryCheck, //pay std::pair<Agent*, sf::Vector2f>
        AgentDesiredBoundaryCheck, //pay std::pair<Agent*, sf::Vector2f>
        LookAheadResponse, //payload std::pair<Agent*, std::array>
        DesiredBoundaryCheckResponse, //payload std::pair<Agent*, std::array>
        AgentUpdate, //pay std::pair<Agent*, sf::Vector2f>
        AgentOffGrid, //payload std::pair<Agent*, std::array>
        AgentCollision, //payload std::pair<Agent*, std::unordered_set<AutoCity::Agent *>
        RoadFlowMap, //payload std::pair<Agent*, std::vector<sf::Angle>     
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