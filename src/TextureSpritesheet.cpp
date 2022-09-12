#include "TextureSpritesheet.hpp"

TextureSpritesheet::~TextureSpritesheet()
{
}

TextureSpritesheet::TextureSpritesheet(
        std::string _name,
        SDL_Texture* _texture)
    :
        name{_name},
        texture{_texture}
{
}
