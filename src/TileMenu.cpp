#include "../include/menus/TileMenu.h"
#include <iostream>

namespace AutoCity {

    TileMenu::TileMenu(sf::RenderWindow& renderWindow, EventBus& eventBus) : CityObject(renderWindow, eventBus){

    };
    void TileMenu::init(){
        bus.subscribe(AutoCity::EventType::TilesLoaded, [this](const Event& e) { loadTiles(e); });
    };
    void TileMenu::processEvents(const sf::Event& event){
    };
    void TileMenu::update(sf::Time delta){
                
    };
    void TileMenu::draw(){

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
                        allTiles.emplace_back(rotated);
                    };
                };
            };
        };
    };
};