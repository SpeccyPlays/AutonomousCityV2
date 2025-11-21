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
            fileOpenHandler();
        };
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            bus.publish(EventType::Save);
        };
        if (ImGui::MenuItem("Save As..")) {
            fileSaveAsHandler();
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
    void MainMenu::fileOpenHandler(){
        NFD_Init();
        nfdu8char_t *outPath;
        nfdu8filteritem_t filters[1] = { {"AutoCity file", "json"}};
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
        if (result == NFD_OKAY){
            std::string fileName = outPath;
            NFD_FreePathU8(outPath);
            Event openEvent = {EventType::Open, fileName};
            bus.publish(openEvent);
        } 
        else if (result == NFD_CANCEL){
            puts("User pressed cancel.");
        } 
        else{
            printf("Error: %s\n", NFD_GetError());
        }
        NFD_Quit();
    };
    void MainMenu::fileSaveAsHandler(){
        NFD_Init();

        nfdchar_t* savePath;
        nfdfilteritem_t filterItem[1] = {{"AutoCity file", "json"}};

        // show the dialog
        nfdresult_t result = NFD_SaveDialog(&savePath, filterItem, 1, NULL, "Untitled.json");
        if (result == NFD_OKAY) {
            std::string fileName = savePath;
            Event saveAsEvent = {EventType::SaveAs, fileName};
            bus.publish(saveAsEvent);
            // remember to free the memory (since NFD_OKAY is returned)
            NFD_FreePath(savePath);
        } 
        else if (result == NFD_CANCEL) {
            puts("User pressed cancel.");
        } 
        else {
            printf("Error: %s\n", NFD_GetError());
        }
        // Quit NFD
        NFD_Quit();
    }
};