#include "Spritesheet.hpp"

Spritesheet::~Spritesheet()
{
}

Spritesheet::Spritesheet(
        std::string _name,
        SDL_Texture* _texture)
    :
        name{_name},
        texture{_texture}
{
}
