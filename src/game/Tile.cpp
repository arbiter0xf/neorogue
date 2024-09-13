#include "Sdlw.hpp"
#include "Tile.hpp"

Tile::~Tile()
{
}

Tile::Tile(
        std::string _name,
        std::shared_ptr<SDL_Texture> _texture,
        const int _sheetX,
        const int _sheetY,
        const int _sheetW,
        const int _sheetH,
        const int _tiledGid,
        const int _tiledId)
        :
            name{_name},
            sheetTexture{_texture},
            sheetX{_sheetX},
            sheetY{_sheetY},
            sheetW{_sheetW},
            sheetH{_sheetH},
            tiledGid{_tiledGid},
            tiledId{_tiledId}
{
}

std::string Tile::getName() const
{
    return name;
}

std::shared_ptr<SDL_Texture> Tile::getSheetTexture()
{
    return sheetTexture;
}

int Tile::getSheetX() const
{
    return sheetX;
}

int Tile::getSheetY() const
{
    return sheetY;
}

int Tile::getSheetW() const
{
    return sheetW;
}

int Tile::getSheetH() const
{
    return sheetH;
}

int Tile::getTiledGid() const
{
    return tiledGid;
}

int Tile::getTiledId() const
{
    return tiledId;
}

std::string Tile::getDump()
{
    std::string dump;

    dump += "name: " + getName() + "\n";
    dump += "x: " + std::to_string(getSheetX()) + "\n";
    dump += "y: " + std::to_string(getSheetY()) + "\n";
    dump += "w: " + std::to_string(getSheetW()) + "\n";
    dump += "h: " + std::to_string(getSheetH()) + "\n";
    dump += "tiledGid: " + std::to_string(getTiledGid()) + "\n";
    dump += "tiledId: " + std::to_string(getTiledId()) + "\n";

    return dump;
}
