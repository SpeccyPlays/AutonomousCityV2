#include "../include/menus/mainMenu.h"

namespace AutoCity {

    MainMenu::MainMenu(sf::RenderWindow& renderWindow) : window(renderWindow) {
            
    };
    int MainMenu::init(){
        if (!ImGui::SFML::Init(window)) {
            return 1;
        };
        return 0;
    };
    void MainMenu::processEvents(const sf::Event& event){
        ImGui::SFML::ProcessEvent(window, event);
    }
    void MainMenu::update(sf::Time deltaClock){
        ImGui::SFML::Update(window, deltaClock);
        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();
    };
    void MainMenu::draw(){
        ImGui::SFML::Render(window);
    };
};