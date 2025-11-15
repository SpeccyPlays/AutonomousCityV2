#include "../include/menus/mainMenu.h"

namespace AutoCity {

    MainMenu::MainMenu(sf::RenderWindow& renderWindow, EventBus& eventBus) : CityObject(renderWindow, eventBus) {
            
    };
    void MainMenu::init(){
    };
    void MainMenu::processEvents(const sf::Event& event){
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
            if (ImGui::BeginMenu("Add..")){
                ImGui::EndMenu();
            };
            if (ImGui::BeginMenu("Debug")){
                showToolsOptions();
                ImGui::EndMenu();
            };
            ImGui::EndMainMenuBar();
        };
    };
    void MainMenu::showFileOptions(){
        ImGui::MenuItem("(Main menu)", NULL, false, false);
        if (ImGui::MenuItem("New")) {
            bus.publish(EventType::New);
        };
        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            std::string fileName = "";
            Event openEvent = {EventType::Open, fileName};
            bus.publish(openEvent);
        };
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            bus.publish(EventType::Save);
        };
        if (ImGui::MenuItem("Save As..")) {
            std::string fileName = "";
            Event saveAsEvent = {EventType::SaveAs, fileName};
            bus.publish(saveAsEvent);
        };
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
            window.close();
        }
    };
    void MainMenu::showToolsOptions(){
        ImGui::MenuItem("(Debug options)", NULL, false, false);
        if (ImGui::Checkbox("Debug Grid", &debugGrid)) {
            bus.publish(EventType::DebugGrid);
        };
        if (ImGui::Checkbox("Debug Agents", &debugAgents)) {
            bus.publish(EventType::DebugAgents);
        };
    };
};