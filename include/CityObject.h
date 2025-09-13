#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

namespace AutoCity {
    class CityObject {
        protected:
                sf::RenderWindow& window;  
        public:
            CityObject(sf::RenderWindow& win) : window(win) {};
            virtual ~CityObject() = default;
            virtual void processEvents(const sf::Event& event) = 0;
            virtual void update(sf::Time delta) = 0;
            virtual void draw() = 0;
    };
};