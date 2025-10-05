#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include "EventBus.h"

namespace AutoCity {
    class CityObject {
        protected:
                sf::RenderWindow& window;
                EventBus& bus;
        private:
            bool debug = false;
        public:
            CityObject(sf::RenderWindow& win, EventBus& eventBus) : window(win), bus(eventBus) {};
            virtual ~CityObject() = default;
            virtual void init() = 0;
            virtual void processEvents(const sf::Event& event) = 0;
            virtual void update(sf::Time delta) = 0;
            virtual void draw() = 0;
            bool getDebug(){
                return debug;
            };
            void toggleDebug(){
                debug = !debug;
            };
    };
};