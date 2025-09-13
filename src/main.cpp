#include <imgui.h>
#include <imgui-SFML.h>
#include "../include/menus/mainMenu.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main() {
    //It's gonna be as big as we can
    sf::Vector2u screenSize = sf::VideoMode::getDesktopMode().size;
    sf::RenderWindow window(sf::VideoMode(screenSize), "Autononmous City V2");
    window.setFramerateLimit(60);
    AutoCity::MainMenu mainMenu(window);
    mainMenu.init();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            mainMenu.processEvents(*event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            };
        };
        mainMenu.update(deltaClock.restart());
        window.clear();
        mainMenu.draw();
        window.display();
    };

    ImGui::SFML::Shutdown();
};