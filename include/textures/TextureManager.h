#pragma once
#include <map>
#include <SFML/Graphics.hpp>

namespace AutoCity {
    
    class TextureManager {

        private:
            static std::map<std::string, sf::Texture> textures;
        
        public:
            static sf::Texture& getTexture(std::string const& filename);
    };
};