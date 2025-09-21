#include "../include/controllers/CityObjectController.h"
#include "../include/menus/MainMenu.h"
#include "../include/menus/TileMenu.h"
#include <iostream>

namespace AutoCity {

    CityObjectController::CityObjectController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){
        objects.push_back(std::make_unique<MainMenu>(window, bus));
        objects.push_back(std::make_unique<TileMenu>(window, bus));
    };
    void CityObjectController::init(){
        for (auto& object : objects){
            object->init();
        };
        bus.subscribe(AutoCity::EventType::TileSelected, [this](const Event& e) { updateTileSelected(e); });
    };
    void CityObjectController::processEvents(const sf::Event& event){
        for (auto& object : objects){
            object->processEvents(event);
        };
        if (event.is<sf::Event::MouseButtonPressed>()){
            auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent->button == sf::Mouse::Button::Right && tileSelected && selectedTile != nullptr){
                deselectTile();
            }
            else if (mouseEvent->button == sf::Mouse::Button::Left && tileSelected && selectedTile != nullptr){
                std::vector<std::pair<Tile, sf::Vector2u>> tileInfo;
                //do this to make a new tile from the pointer
                Tile* tilePtr = selectedTile.get();
                Tile newTile = *tilePtr; 
                tileInfo.emplace_back(newTile, mouseEvent->position); 
                Event event = {EventType::TileAdded, tileInfo};
                bus.publish(event);
            };
        };
    };
    void CityObjectController::update(sf::Time delta){
        for (auto& object : objects){
            object->update(delta);
        };
    };
    void CityObjectController::draw(){
        for (auto& object : objects){
            object->draw();
        };
        if (tileSelected && selectedTile != nullptr){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            selectedTile->sprite.setPosition(static_cast<sf::Vector2f>(mousePos));
            window.draw(selectedTile->sprite);
        };
    };
    void CityObjectController::updateTileSelected(const Event& e){
        tileSelected = true;
        auto tilePtr = std::any_cast<AutoCity::Tile*>(e.payload);
        if (tilePtr) {
            selectedTile = std::make_unique<AutoCity::Tile>(*tilePtr);
            tileSelected = true;
        }
    };
    void CityObjectController::deselectTile(){
        tileSelected = false;
        selectedTile = nullptr; 
    };
};