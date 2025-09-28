#include <imgui.h>
#include <imgui-SFML.h>
#include "../include/CityGrid/Tile.h"
#include "../include/controllers/CityObjectController.h"
#include "../include/textures/TextureManager.h"
#include "../include/CityGrid/TileManager.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <cmath>

int main() {
    //It's gonna be as big as we can
    sf::Vector2u screenSize = sf::VideoMode::getDesktopMode().size;
    sf::RenderWindow window(sf::VideoMode(screenSize), "Autononmous City V2");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) {
        return 1;
    };
    AutoCity::TextureManager texManager;
    AutoCity::EventBus cityEvents;
    AutoCity::TileManager tileManager;
    const std::string roadTexures= "include/textures/RoadsTextures.png";
    AutoCity::CityObjectController cityController(window, cityEvents);
    cityController.init();
    tileManager.setEventBus(cityEvents);
    tileManager.setBasicTiles(texManager.getTexture(roadTexures));

    sf::Clock deltaClock;
    //set all menu backgrounds so not blended
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) {
                window.close();
            };
            cityController.processEvents(*event);
        };
        ImGui::SFML::Update(window, deltaClock.restart());
        cityController.update(deltaClock.restart());
        window.clear();
        cityController.draw();
        ImGui::SFML::Render(window);
        window.display();
    };

    ImGui::SFML::Shutdown();
};