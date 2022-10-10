#ifndef TILE_HPP_DEFINED
#define TILE_HPP_DEFINED

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "Constants.hpp"
#include "Spritesheet.hpp"

class Tile;

using tile_pool = std::unordered_map<int, Tile>;
using tile_id_map = std::array<std::string, g_constants::TILE_AMOUNT>;

class Tile {
public:
    Tile() = default;
    ~Tile();
    Tile(
            std::string _name,
            std::shared_ptr<SDL_Texture> _texture,
            const int _sheetX,
            const int _sheetY,
            const int _sheetW,
            const int _sheetH,
            const int _tiledGid,
            const int _tiledId);

    const int getScreenX() const;
    const int getScreenY() const;
    const std::string getName() const;
    std::shared_ptr<SDL_Texture> getSheetTexture();
    const int getSheetX() const;
    const int getSheetY() const;
    const int getSheetW() const;
    const int getSheetH() const;
    const int getTiledGid() const;
    const int getTiledId() const;
    std::string getDump();

private:
    std::shared_ptr<SDL_Texture> sheetTexture;
    std::string name;
    int sheetX;
    int sheetY;
    int sheetW;
    int sheetH;
    int tiledGid;
    int tiledId;
};

#endif
