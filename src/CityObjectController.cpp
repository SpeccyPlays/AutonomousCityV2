#include "CityObjectController.h"
#include "../include/menus/MainMenu.h"

namespace AutoCity {

    CityObjectController::CityObjectController(sf::RenderWindow& window, AutoCity::EventBus& bus) : CityObject(window, bus){
        objects.push_back(std::make_unique<MainMenu>(window, bus));
    };
    void CityObjectController::init(){
        for (auto& object : objects){
            object->init();
        };
    };
    void CityObjectController::processEvents(const sf::Event& event){
        for (auto& object : objects){
            object->processEvents(event);
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
    };
};