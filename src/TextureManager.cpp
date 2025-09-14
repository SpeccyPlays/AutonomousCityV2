#include "../include/textures/TextureManager.h"
#include <assert.h>
#include <iostream>

namespace AutoCity {

    std::map<std::string, sf::Texture> TextureManager::textures;

    sf::Texture& TextureManager::getTexture(std::string const& filename){
        auto keyValuePair = textures.find(filename);
        if (keyValuePair != textures.end()){
            return keyValuePair->second;
        }
        else {
            auto& texture = textures[filename];
            if (!texture.loadFromFile(filename)){
                std::cout << "Texture not loaded: " << filename << std::endl;
            };
            return texture;
        };
    };
};