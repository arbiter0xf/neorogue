#ifndef SPRITESHEET_HPP_DEFINED
#define SPRITESHEET_HPP_DEFINED

#include <SDL2/SDL.h>
#include <string>

#include "Json.hpp"

class Spritesheet {
public:
    Spritesheet() = default;
    ~Spritesheet();
    Spritesheet(std::string _name);

    std::string getName(void);
    SDL_Texture* getTexture(void);
    boost::json::value getJson(void);

private:
    std::string name;
    SDL_Texture* texture;
    boost::json::value jsonValue;

    void loadTexture(std::string pathImage);
    void loadJson(std::string pathJson);
};

#endif
