#include "../include/menus/TileMenu.h"
#include <iostream>
#include <cmath>

namespace AutoCity {

    TileMenu::TileMenu(sf::RenderWindow& renderWindow, EventBus& eventBus) : CityObject(renderWindow, eventBus){

    };
    void TileMenu::init(){
        bus.subscribe(AutoCity::EventType::TilesLoaded, [this](const Event& e) { loadTiles(e); });
    };
    void TileMenu::processEvents(const sf::Event& event){
    };
    void TileMenu::update(sf::Time delta){
        showTileMenu();
    };
    void TileMenu::draw(){
        //ImGui doesn't handle sprite rotation so we're going to have to fudge adding sprites
        //just a plus button in the draw menu with sfml manually drawing the tile sprite next to it in this function
        if (allTiles.size() > 0){
            float tileX = window.getSize().x - TileManager::tileSize.x;
            float tileY = 0 + 43.f; //
            for (auto& tile : allTiles){
                tile.sprite.setPosition({tileX, tileY});
                window.draw(tile.sprite);
                tileY += 36.f;
            };
        };
    };
    void TileMenu::showTileMenu(){
        if (allTiles.size() > 0){
            //Make sure menu is fixed size and stays to the right
            ImGui::SetNextWindowSize(ImVec2(TileManager::tileSize.x * 2, window.getSize().y));
            ImGui::SetNextWindowPos(ImVec2(window.getSize().x - (TileManager::tileSize.x * 3), 0));     
            ImGui::Begin("Tiles:", nullptr,
                        ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoMove);
            for (auto& tile : allTiles){
                std::string id = std::string("Add-") + "##" + tile.toString();
                if (ImGui::Button(id.c_str(), TileManager::tileSize)) {
                    Event event = {EventType::TileSelected, &tile};
                    bus.publish(event);
                };
            };
            ImGui::End();
        };
    };
    void TileMenu::loadTiles(const Event& e){
        const auto& ev = std::any_cast<std::map<TileType, std::map<TileSubType, Tile>>>(e.payload);
        for (const auto& [type, subMap] : ev) {
            
            for (const auto& [subType, baseTile] : subMap) {
                if (type == TileType::Default || subType == TileSubType::NoFlow){
                    continue;
                };
                if (type != TileType::Road){
                    continue;
                };
                 //tile only needs 1 rotation as either north & south or east and west
                if (subType == TileSubType::Straight){
                    Tile newTile = baseTile;
                    //have to do this weird new sprite thing or might get some weirdness when displaying
                    newTile.sprite = sf::Sprite(baseTile.sprite.getTexture(), baseTile.sprite.getTextureRect());
                    newTile.rotation = sf::degrees(0);
                    //do this so the origin is centre of the tile and not the top left
                    newTile.sprite.setOrigin(newTile.origin);
                    newTile.sprite.setRotation(newTile.rotation);
                    allTiles.emplace_back(newTile);
                    //now rotate it once
                    Tile newTile2 = baseTile;
                    //have to do this weird new sprite thing or might get some weirdness when displaying
                    newTile2.sprite = sf::Sprite(baseTile.sprite.getTexture(), baseTile.sprite.getTextureRect());
                    float angle = 90;
                    newTile2.rotation = sf::degrees(angle);
                    //do this so the origin is centre of the tile and not the top left
                    newTile2.sprite.setOrigin(newTile2.origin);
                    newTile2.sprite.setRotation(newTile2.rotation);
                    newTile2.flowMap = rotateFlowMap(newTile2.flowMap, angle);
                    allTiles.emplace_back(newTile2);
                }
                else {
                    for (int angle = 0; angle < 360; angle += 90) {
                        Tile rotated = baseTile;
                        rotated.rotation = sf::degrees(angle);
                        //have to do this weird new sprite thing or might get some weirdness when displaying
                        rotated.sprite = sf::Sprite(baseTile.sprite.getTexture(), baseTile.sprite.getTextureRect());
                        //do this so the origin is centre of the tile and not the top left
                        rotated.sprite.setOrigin(rotated.origin);
                        rotated.sprite.setRotation(rotated.rotation);
                        rotated.flowMap = rotateFlowMap(rotated.flowMap, angle);
                        allTiles.emplace_back(rotated);
                    };
                };
            };
        };
    };
    std::vector<sf::Angle> TileMenu::rotateFlowMap(std::vector<sf::Angle> map, int angle){
        std::vector<sf::Angle> newFlowMap = map;
        for (sf::Angle& flowAngle : newFlowMap){
            sf::Angle newAngle = flowAngle + sf::degrees(angle);
            float wrapped = fmod(newAngle.asDegrees(), 360.f); //Wrap into [0, 360)
            if (wrapped < 0) wrapped += 360.f;
            flowAngle = sf::degrees(wrapped);
        }
        return newFlowMap;
    };
};