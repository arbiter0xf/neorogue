#include "Tile.hpp"

Tile::~Tile()
{
}

Tile::Tile(
        const int _screenX,
        const int _screenY,
        std::string _name,
        std::shared_ptr<SDL_Texture> _texture,
        const int _sheetX,
        const int _sheetY,
        const int _sheetW,
        const int _sheetH)
        :
            screenX{_screenX},
            screenY{_screenY},
            name{_name},
            sheetTexture{_texture},
            sheetX{_sheetX},
            sheetY{_sheetY},
            sheetW{_sheetW},
            sheetH{_sheetH}
{
}

const int Tile::getScreenX() const
{
    return screenX;
}

const int Tile::getScreenY() const
{
    return screenY;
}

const std::string Tile::getName() const
{
    return name;
}

std::shared_ptr<SDL_Texture> Tile::getSheetTexture()
{
    return sheetTexture;
}

const int Tile::getSheetX() const
{
    return sheetX;
}

const int Tile::getSheetY() const
{
    return sheetY;
}

const int Tile::getSheetW() const
{
    return sheetW;
}

const int Tile::getSheetH() const
{
    return sheetH;
}
