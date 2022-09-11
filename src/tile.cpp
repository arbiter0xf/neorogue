#include "tile.hpp"

Tile::~Tile()
{
}

Tile::Tile(SDL_Texture* _texture,
        const int _x,
        const int _y,
        const int _w,
        const int _h)
        : sheetTexture{_texture}, x{_x}, y{_y}, w{_w}, h{_h}
{
}

SDL_Texture* Tile::getSheetTexture()
{
    return sheetTexture;
}

const int Tile::getX() const
{
    return x;
}

const int Tile::getY() const
{
    return y;
}

const int Tile::getW() const
{
    return w;
}

const int Tile::getH() const
{
    return h;
}
