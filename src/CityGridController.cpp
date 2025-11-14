#include "../include/controllers/CityGridController.h"
#include "../include/agents/Agent.h"
#include "../include/CityGrid/json.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>

using json = nlohmann::json;

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
        bus.subscribe(AutoCity::EventType::Save, [this](const Event& e) { saveGrid(e); });
        bus.subscribe(AutoCity::EventType::RemoveAgent, [this](const Event& e) { removeAgent(e); });
        bus.subscribe(AutoCity::EventType::AgentGetTile, [this](const Event& e) { sendTileForAgent(e); });
        bus.subscribe(AutoCity::EventType::AgentLookAheadBoundaryCheck, [this](const Event& e) { agentLookAhead(e); });
        bus.subscribe(AutoCity::EventType::AgentDesiredBoundaryCheck, [this](const Event& e) { agentDesiredBoundaryCheck(e); });
        bus.subscribe(AutoCity::EventType::AgentCollisionCheck, [this](const Event& e) { sendOccupants(e); });
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
                    sf::Vector2f drawPos = {float(j * TileManager::tileSize.x), float(i * TileManager::tileSize.y)};
                    //origin will be center of tile so add half tilesize
                    sf::Vector2f backFromOrigin = {float(TileManager::tileSize.x / 2), float(TileManager::tileSize.y / 2)};
                    drawPos += backFromOrigin;
                    tile.sprite.setPosition(drawPos);
                    window.draw(tile.sprite);
                };
                //Show if a cell has occupants 
                if (CityObject::getDebug() && grid[i][j].occupants.size() > 0){
                    sf::Vector2f drawPos = {float(j * TileManager::tileSize.x), float(i * TileManager::tileSize.y)};
                    sf::RectangleShape square(sf::Vector2f(AutoCity::TileManager::tileSize.x, AutoCity::TileManager::tileSize.y));
                    int amountOfRed = 128 + grid[i][j].occupants.size() * 10;
                    if (amountOfRed > 255){
                        amountOfRed = 255;
                    }
                    square.setFillColor(sf::Color(255, 0, 0, amountOfRed));
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
            grid[gridPos.y][gridPos.x].tile = tile;
        };
    };
    sf::Vector2u CityGridController::pixelToGridPos(sf::Vector2f pos){
        unsigned int xPos = int(pos.x / TileManager::tileSize.x);
        unsigned int yPos = int(pos.y / TileManager::tileSize.y);
        sf::Vector2u gridPos = {xPos, yPos};
        return gridPos;
    };
    void CityGridController::removeAgent(const Event& e){
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f pos = agentInfo.second;
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
        sf::Vector2u gridPos = pixelToGridPos(pos);
        addAgent(agent, gridPos);      
    };
    std::array<bool, 4> CityGridController::isAgentOnGrid(Agent *agent, sf::Vector2f agentPos){
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
        return offGrid;     
    };
    void CityGridController::sendOccupants(const Event& e){
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f pos = agentInfo.second;
        sf::Vector2u agentGridPos = pixelToGridPos(pos);
        Event event = {EventType::AgentCollisionCheckResponse, std::pair{agent, grid[agentGridPos.y][agentGridPos.x].occupants}};
        bus.publish(event);
    };
    void CityGridController::sendTileForAgent(const Event& e){
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f pos = agentInfo.second;
        sf::Vector2u gridPos = pixelToGridPos(pos);
        Event event = {EventType::AgentTile, std::pair{agent, grid[gridPos.y][gridPos.x].tile}};
        bus.publish(event);
    };
    void CityGridController::agentLookAhead(const Event& e){
        //Receives lookahead value from agentcontroller to see if agent is heading off grid
        //offGrid array is Top, Right, Bottom, Left
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f agentPos = agentInfo.second;
        std::array<bool, 4> offGrid = isAgentOnGrid(agent, agentPos);
        Event event = {EventType::LookAheadResponse, std::pair{agent, offGrid}};
        bus.publish(event);
    };
    void CityGridController::agentDesiredBoundaryCheck(const Event& e){
        //Receives lookahead value from agentcontroller to see if agent is heading off grid
        //offGrid array is Top, Right, Bottom, Left
        auto agentInfo = std::any_cast<std::pair<Agent*, sf::Vector2f>>(e.payload);
        Agent* agent = agentInfo.first;
        sf::Vector2f agentPos = agentInfo.second;
        std::array<bool, 4> offGrid = isAgentOnGrid(agent, agentPos);
        Event event = {EventType::DesiredBoundaryCheckResponse, std::pair{agent, offGrid}};
        bus.publish(event);
    };
    void CityGridController::saveGrid(const Event& e){
        json json;
        json["gridwidth"] = gridSize.x;
        json["gridheight"] = gridSize.y;
        json["gridstartx"] = gridStart.x;
        json["gridstarty"] = gridStart.y;
        json["gridendx"] = gridEnd.x;
        json["gridendy"] = gridEnd.y;

        for (int i = 0; i < grid.size(); i++){
            for (int j = 0; j < grid[i].size(); j++){
                {
                    const Tile &tile = grid[i][j].tile;
                    json["cells"].push_back({
                        {"x", j},
                        {"y", i},
                        {"type", static_cast<int>(tile.type)},
                        {"subtype", static_cast<int>(tile.subType)},
                        {"originx", tile.origin.x},
                        {"originy", tile.origin.y},  
                        {"rotation", tile.rotation.asDegrees()},
                        {"flowmap0", tile.flowMap[0].asDegrees()},
                        {"flowmap1", tile.flowMap[1].asDegrees()},
                        {"flowmap2", tile.flowMap[2].asDegrees()},
                        {"flowmap3", tile.flowMap[3].asDegrees()},
                    });
                }
            }
        };
        std::ofstream file("test.json");
        if (!file.is_open()){
            std::cout << "Problem opening file" << std::endl;
            return;
        };
        file << json.dump(2);
       std::cout << "Problem opening file" << std::endl;
    };
};