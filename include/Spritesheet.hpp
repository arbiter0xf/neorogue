#ifndef SPRITESHEET_HPP_DEFINED
#define SPRITESHEET_HPP_DEFINED

#include <SDL2/SDL.h>
#include <memory>
#include <string>

#include "Json.hpp"

const int SPRITESHEET_POOL_SIZE = 8;

class Spritesheet;

using spritesheet_pool = std::array<Spritesheet, SPRITESHEET_POOL_SIZE>;

class Spritesheet {
public:
    Spritesheet() = default;
    ~Spritesheet() = default;
    Spritesheet(std::string _name);

    std::string getName(void);
    std::shared_ptr<SDL_Texture> getTexture(void);
    boost::json::value getJson(void);

    static void loadSpritesheets(spritesheet_pool& spritesheetPool);

    static const std::array<std::string, SPRITESHEET_POOL_SIZE> spritesheetNames;

private:
    std::string name;
    std::shared_ptr<SDL_Texture> texture;
    boost::json::value jsonValue;

    void loadTexture(std::string pathImage);
    void loadJson(std::string pathJson);
};

#endif
