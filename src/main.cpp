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
    if (!ImGui::SFML::Init(window)) {
        return 1;
    };
    AutoCity::MainMenu mainMenu(window);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) {
                window.close();
            };
        };
        ImGui::SFML::Update(window, deltaClock.restart());
        mainMenu.update(deltaClock.restart());
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    };

    ImGui::SFML::Shutdown();
};