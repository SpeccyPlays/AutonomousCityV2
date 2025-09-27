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

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2i gridStart = AutoCity::TileManager::tileSize;
    sf::Vector2i gridEnd = {static_cast<int>(windowSize.x - (AutoCity::TileManager::tileSize.x * 3)),static_cast<int>(windowSize.y - AutoCity::TileManager::tileSize.y)};
    sf::Vector2i gridSize = {(gridEnd.x - gridStart.x) / AutoCity::TileManager::tileSize.x, (gridEnd.y - gridStart.y) / AutoCity::TileManager::tileSize.y};
    std::cout << "Grid X: " << gridSize.x << " Grid Y: " << gridSize.y << std::endl;
    sf::Clock deltaClock;
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
        ImGui::SFML::Render(window);
        cityController.draw();
        for (auto col = 0; col <= gridSize.x; col++){
            std::array line =
                {
                    sf::Vertex{sf::Vector2f(col * AutoCity::TileManager::tileSize.x, gridStart.y)},
                    sf::Vertex{sf::Vector2f(col * AutoCity::TileManager::tileSize.x, gridSize.y * AutoCity::TileManager::tileSize.y)}
                };
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }
        for (auto rowCount = 0; rowCount <= gridSize.y; rowCount++){
            std::array line =
                {
                    sf::Vertex{sf::Vector2f(gridStart.x, rowCount * AutoCity::TileManager::tileSize.y)},
                    sf::Vertex{sf::Vector2f(gridSize.x * AutoCity::TileManager::tileSize.x, rowCount * AutoCity::TileManager::tileSize.y)}
                };
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }
        window.display();
    };

    ImGui::SFML::Shutdown();
};