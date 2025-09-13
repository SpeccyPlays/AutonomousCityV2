#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

namespace AutoCity {
    class Menu {
        public:
            virtual ~Menu() = default;

            virtual void processEvents(const sf::Event& event) = 0;
            virtual void update() = 0;
    };
};