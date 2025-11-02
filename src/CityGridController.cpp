#include "../include/controllers/CityGridController.h"
#include "../include/agents/Agent.h"
#include <iostream>
#include <math.h>
#include <algorithm>

namespace AutoCity {

    class Agent;
    
    CityGridController::CityGridController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus) {
        sf::Vector2u windowSize = window.getSize();
        //We're going to draw the grid using the whole window
        //however, we will not allow tiles to be placed in the menu areas
        gridStart = {0, AutoCity::TileManager::tileSize.y};
        //below just so visually it's not off screen
        gridEnd = {static_cast<int>(windowSize.x - (AutoCity::TileManager::tileSize.x * 3)),static_cast<int>(windowSize.y - (AutoCity::TileManager::tileSize.y * 4))};
        gridSize = {windowSize.x / AutoCity::TileManager::tileSize.x, windowSize.y / AutoCity::TileManager::tileSize.y};
        newGrid();
    };
    void CityGridController::init(){
        this->toggleDebug(); //should switch debug on
        bus.subscribe(AutoCity::EventType::TileAdded, [this](const Event& e) { addTile(e); });
        bus.subscribe(AutoCity::EventType::New, [this](const Event& e) { newGrid(e); });
        bus.subscribe(AutoCity::EventType::RemoveAgent, [this](const Event& e) { removeAgent(e); });
        bus.subscribe(AutoCity::EventType::AgentLookAhead, [this](const Event& e) { agentLookAhead(e); });
        bus.subscribe(AutoCity::EventType::AgentUpdate, [this](const Event& e) { agentUpdate(e); });
        bus.subscribe(AutoCity::EventType::DebugGrid, [this](const Event& e) { this->toggleDebug(); });
    };
    void CityGridController::processEvents(const sf::Event& event){
    };
    void CityGridController::update(sf::Time delta) {
    };
    void CityGridController::newGrid(){
        Tile defaultTile = TileManager::getTile(TileType::Default, TileSubType::NoFlow);
        Cell cell = {defaultTile};
        grid.resize(gridSize.y, std::vector<Cell>(gridSize.x, cell));
    };
    void CityGridController::newGrid(const Event& e){
        Tile defaultTile = TileManager::getTile(TileType::Default, TileSubType::NoFlow);
        Cell cell = {defaultTile};
        std::vector<std::vector<Cell>> newGrid;
        newGrid.resize(gridSize.y, std::vector<Cell>(gridSize.x, cell));
        grid = newGrid;
    };
    void CityGridController::draw(){
        drawGrid();
        drawTiles();
    };
    void CityGridController::drawGrid(){
        if (CityObject::getDebug()){
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
    };
    void CityGridController::drawTiles(){
        for (int i = 0; i < grid.size(); i++){
            for (int j = 0; j < grid[i].size(); j++){
                Tile tile = grid[i][j].tile;
                if (tile.type != TileType::Default){
                    sf::Vector2f drawPos = {(float)gridStart.x + j * TileManager::tileSize.x, (float)gridStart.y + i * TileManager::tileSize.y};
                    tile.sprite.setPosition(drawPos);
                    window.draw(tile.sprite); 
                };
                //Show if a cell has occupants 
                if (CityObject::getDebug() && grid[i][j].occupants.size() > 0){
                    sf::Vector2f drawPos = {(float)gridStart.x + j * TileManager::tileSize.x, (float)gridStart.y + i * TileManager::tileSize.y};
                    sf::RectangleShape square(sf::Vector2f(AutoCity::TileManager::tileSize.x, AutoCity::TileManager::tileSize.y));
                    square.setFillColor(sf::Color(255, 0, 0, 128));
                    square.setPosition(drawPos);
                    window.draw(square);
                };
            };
        };
    };
    void CityGridController::addTile(const Event& e){
        auto placementInfo = std::any_cast<std::pair<Tile, sf::Vector2i>>(e.payload);
        Tile tile = placementInfo.first;
        //Get 'true' mouse pos
        sf::Vector2f tilePos = window.mapPixelToCoords(placementInfo.second);
        if (!checkInGrid(tilePos)){
            std::cout << "Tile out of bounds" << std::endl;
        }
        else {
            addTileToGrid(tilePos, tile);
        };
    };
    bool CityGridController::checkInGrid(sf::Vector2f pos){
        if (pos.x < gridStart.x || pos.y < gridStart.y || pos.x >= gridEnd.x || pos.y >= gridEnd.y){
            return false;
        };
        return true;
    };
    void CityGridController::addTileToGrid(sf::Vector2f pos, Tile tile){
        sf::Vector2u gridPos = pixelToGridPos(pos);
        if (gridPos.y < grid.size() && gridPos.x < grid[gridPos.y].size()) {
            tile.sprite.setOrigin({0, 0});
            grid[gridPos.y][gridPos.x].tile = tile;
        };
    };
    sf::Vector2u CityGridController::pixelToGridPos(sf::Vector2f pos){
        unsigned int xPos = static_cast<unsigned int>(std::floor(pos.x / TileManager::tileSize.x));
        unsigned int yPos = static_cast<unsigned int>(std::floor(pos.y / TileManager::tileSize.y));
        sf::Vector2u gridPos = {xPos, yPos};
        return gridPos;
    };
    void CityGridController::removeAgent(const Event& e){
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f pos = agentInfo.second;
        if (!isAgentOnGrid(agent, pos)){
            return;
        }
        sf::Vector2u gridPos = pixelToGridPos(pos);
        grid[gridPos.y][gridPos.x].occupants.erase(agent);
    };
    void CityGridController::addAgent(Agent *agent, sf::Vector2u agentGridPos){
        grid[agentGridPos.y][agentGridPos.x].occupants.emplace(agent);
    };
    void CityGridController::agentUpdate(const Event& e){
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f pos = agentInfo.second;
        if (!isAgentOnGrid(agent, pos)){
            return;
        }
        sf::Vector2u gridPos = pixelToGridPos(pos);
        addAgent(agent, gridPos);
        if (!isAgentAlone(agent, gridPos)){
            return;
        }
        sendFlowMap(agent, gridPos);
    };
    bool CityGridController::isAgentOnGrid(Agent *agent, sf::Vector2f agentPos){
        //offGrid array is Top, Right, Bottom, Left
        std::array<bool, 4> offGrid = {false, false, false, false};
        if (agentPos.y < gridStart.y){
            offGrid[0] = true;
        };
        if (agentPos.x > gridEnd.x){
            offGrid[1] = true;
        };
        if (agentPos.y > gridEnd.y){
            offGrid[2] = true;
        };
        if (agentPos.x < gridStart.x){
            offGrid[3] = true;
        };
        int offCount = std::count(offGrid.begin(), offGrid.end(), true);
        if (offCount > 0){
            Event event = {EventType::AgentOffGrid, std::pair{agent, offGrid}};
            bus.publish(event);
            return false;
        }
        return true;        
    };
    bool CityGridController::isAgentAlone(Agent *agent, sf::Vector2u agentGridPos){
        if (grid[agentGridPos.y][agentGridPos.x].occupants.size() > 1){
            Event event = {EventType::AgentCollision, std::pair{agent, grid[agentGridPos.y][agentGridPos.x].occupants}};
            bus.publish(event);
            return false;
        }
        return true;
    };
    void CityGridController::sendFlowMap(Agent *agent, sf::Vector2u agentGridPos){
        Event event = {EventType::RoadFlowMap, std::pair{agent, grid[agentGridPos.y][agentGridPos.x].tile.flowMap}};
        bus.publish(event);
    }
    void CityGridController::agentLookAhead(const Event& e){
        //Receives lookahead value from agentcontroller to see if agent is heading off grid
        //offGrid array is Top, Right, Bottom, Left
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f agentPos = agentInfo.second;
        std::array<bool, 4> offGrid = {false, false, false, false};
        if (agentPos.y < gridStart.y){
            offGrid[0] = true;
        };
        if (agentPos.x > gridEnd.x){
            offGrid[1] = true;
        };
        if (agentPos.y > gridEnd.y){
            offGrid[2] = true;
        };
        if (agentPos.x < gridStart.x){
            offGrid[3] = true;
        };
        int offCount = std::count(offGrid.begin(), offGrid.end(), true);
        Event event = {EventType::AgentOffGrid, std::pair{agent, offGrid}};
        bus.publish(event);
    }
};