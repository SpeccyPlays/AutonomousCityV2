#include "../include/CityGrid/CityGridController.h"
#include <iostream>

namespace AutoCity {

    CityGridController::CityGridController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus) {
        sf::Vector2u windowSize = window.getSize();
        gridStart = AutoCity::TileManager::tileSize;
        gridEnd = {static_cast<int>(windowSize.x - (AutoCity::TileManager::tileSize.x * 3)),static_cast<int>(windowSize.y - AutoCity::TileManager::tileSize.y)};
        gridSize = {(gridEnd.x - gridStart.x) / AutoCity::TileManager::tileSize.x, (gridEnd.y - gridStart.y) / AutoCity::TileManager::tileSize.y};
    };
    void CityGridController::init(){
        bus.subscribe(AutoCity::EventType::TileAdded,  [this](const Event& e) { addTile(e); });
    };
    void CityGridController::processEvents(const sf::Event& event){
    };
    void CityGridController::update(sf::Time delta) {
    };
    void CityGridController::draw(){
        drawGrid();
    };
    void CityGridController::drawGrid(){
        for (int col = 0; col <= gridSize.x; col++) {
            int x = gridStart.x + col * AutoCity::TileManager::tileSize.x;
            std::array line = {
                sf::Vertex{sf::Vector2f(x, gridStart.y)},
                sf::Vertex{sf::Vector2f(x, gridStart.y + gridSize.y * AutoCity::TileManager::tileSize.y)}
            };
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }

        for (int row = 0; row <= gridSize.y; row++) {
            int y = gridStart.y + row * AutoCity::TileManager::tileSize.y;
            std::array line = {
                sf::Vertex{sf::Vector2f(gridStart.x, y)},
                sf::Vertex{sf::Vector2f(gridStart.x + gridSize.x * AutoCity::TileManager::tileSize.x, y)}
            };
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }

    };
    void CityGridController::addTile(const Event& e){
        const auto& [tile, tilePos] = std::any_cast<std::pair<Tile, sf::Vector2u>>(e.payload);
        if (!checkInGrid(tilePos)){
            std::cout << "Tile out of bounds" << std::endl;
        }
        else {
            std::cout << "Tile placed at:" << std::endl;
            std::cout << "X: " << tilePos.x << " Y: " << tilePos.y << std::endl;
        }
    };
    bool CityGridController::checkInGrid(sf::Vector2u pos){
        if (pos.x < gridStart.x || pos.y < gridStart.y || pos.x >= gridEnd.x || pos.y >= gridEnd.y){
            return false;
        }
        return true;
    };
};