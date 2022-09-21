#ifndef TILE_HPP_DEFINED
#define TILE_HPP_DEFINED

#include <SDL2/SDL.h>
#include <string>

class Spritesheet {
public:
    Spritesheet() = delete;
    ~Spritesheet();
    Spritesheet(
            std::string _name,
            SDL_Texture* _texture);

    std::string getName();
    SDL_Texture* getTexture();

private:
    std::string name;
    SDL_Texture* texture;
};

#endif
