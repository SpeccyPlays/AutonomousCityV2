#include "../include/CityGrid/CityGridController.h"
#include <iostream>
#include <math.h>

namespace AutoCity {

    CityGridController::CityGridController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus) {
        sf::Vector2u windowSize = window.getSize();
        gridStart = {0, AutoCity::TileManager::tileSize.y};
        gridEnd = {static_cast<int>(windowSize.x - (AutoCity::TileManager::tileSize.x * 3)),static_cast<int>(windowSize.y - AutoCity::TileManager::tileSize.y)};
        gridSize = {(gridEnd.x - gridStart.x) / AutoCity::TileManager::tileSize.x, (gridEnd.y - gridStart.y) / AutoCity::TileManager::tileSize.y};
        newGrid();
    };
    void CityGridController::init(){
        bus.subscribe(AutoCity::EventType::TileAdded, [this](const Event& e) { addTile(e); });
        bus.subscribe(AutoCity::EventType::New, [this](const Event& e) { newGrid(e); });
    };
    void CityGridController::processEvents(const sf::Event& event){
    };
    void CityGridController::update(sf::Time delta) {
    };
    void CityGridController::newGrid(){
        Tile defaultTile = TileManager::getTile(TileType::Default, TileSubType::NoFlow);
        grid.resize(gridSize.y, std::vector<Tile>(gridSize.x, defaultTile));
    };
    void CityGridController::newGrid(const Event& e){
        Tile defaultTile = TileManager::getTile(TileType::Default, TileSubType::NoFlow);
        std::vector<std::vector<Tile>> newGrid;
        newGrid.resize(gridSize.y, std::vector<Tile>(gridSize.x, defaultTile));
        grid = newGrid;
    };
    void CityGridController::draw(){
        drawGrid();
        drawTiles();
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

        for (int row = 0; row < gridSize.y; row++) {
            int y = gridStart.y + row * AutoCity::TileManager::tileSize.y;
            std::array line = {
                sf::Vertex{sf::Vector2f(gridStart.x, y)},
                sf::Vertex{sf::Vector2f(gridStart.x + gridSize.x * AutoCity::TileManager::tileSize.x, y)}
            };
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }

    };
    void CityGridController::drawTiles(){
        for (int i = 0; i < grid.size(); i++){
            for (int j = 0; j < grid[i].size(); j++){
                Tile tile = grid[i][j];
                if (tile.type != TileType::Default){
                    sf::Vector2f drawPos = {(float)gridStart.x + j * TileManager::tileSize.x, (float)gridStart.y + i * TileManager::tileSize.y};
                    tile.sprite.setPosition(drawPos);
                    window.draw(tile.sprite);
                };
            };
        };
    };
    void CityGridController::addTile(const Event& e){
        auto placementInfo = std::any_cast<std::pair<Tile, sf::Vector2u>>(e.payload);
        Tile tile = placementInfo.first;
        sf::Vector2u tilePos = placementInfo.second;
        //do this or it looks off by one tile later

        sf::Vector2u snappedPos = {
            tilePos.x,
            tilePos.y
        };
        if (!checkInGrid(snappedPos)){
            std::cout << "Tile out of bounds" << std::endl;
        }
        else {
            addTileToGrid(snappedPos, tile);
        };
    };
    bool CityGridController::checkInGrid(sf::Vector2u pos){
        if (pos.x < gridStart.x || pos.y < gridStart.y || pos.x >= gridEnd.x || pos.y >= gridEnd.y){
            return false;
        };
        return true;
    };
    void CityGridController::addTileToGrid(sf::Vector2u pos, Tile tile){
        sf::Vector2u gridPos = {(pos.x - gridStart.x + (TileManager::tileSize.x / 2) ) / TileManager::tileSize.x, (pos.y - gridStart.y + (TileManager::tileSize.y / 2)) / TileManager::tileSize.y};
        if (gridPos.y < grid.size() && gridPos.x < grid[gridPos.y].size()) {
            tile.sprite.setOrigin({0, 0});
            grid[gridPos.y][gridPos.x] = tile;
        };
    };
};