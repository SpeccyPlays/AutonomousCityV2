#include "../include/menus/mainMenu.h"

namespace AutoCity {

    MainMenu::MainMenu(sf::RenderWindow& renderWindow) : CityObject(renderWindow) {
            
    };
    void MainMenu::processEvents(const sf::Event& event){
        ImGui::SFML::ProcessEvent(window, event);
    };
    void MainMenu::update(sf::Time delta) {
        showMainMenu();
    };
    void MainMenu::draw(){

    };
    void MainMenu::showMainMenu(){
        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File")){
                showFileOptions();
                ImGui::EndMenu();
            };
            if (ImGui::BeginMenu("Tools")){
                showToolsOptions();
                ImGui::EndMenu();
            };
            ImGui::EndMainMenuBar();
        };
    };
    void MainMenu::showFileOptions(){
        ImGui::MenuItem("(Main menu)", NULL, false, false);
        if (ImGui::MenuItem("New")) {};
        if (ImGui::MenuItem("Open", "Ctrl+O")) {};
        if (ImGui::MenuItem("Save", "Ctrl+S")) {};
        if (ImGui::MenuItem("Save As..")) {};
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
            window.close();
        }
    };
    void MainMenu::showToolsOptions(){
        ImGui::MenuItem("(Tools menu)", NULL, false, false);
        if (ImGui::MenuItem("Debug")) {};
    };
};