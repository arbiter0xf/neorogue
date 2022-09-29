#ifndef TILE_HPP_DEFINED
#define TILE_HPP_DEFINED

#include <SDL2/SDL.h>
#include <memory>
#include <string>

#include "Constants.hpp"
#include "Spritesheet.hpp"

class Tile;

using tile_pool = std::array<Tile, g_constants::TILE_POOL_SIZE>;

class Tile {
public:
    Tile() = default;
    ~Tile();
    Tile(
            const int _screenX,
            const int _screenY,
            std::string _name,
            std::shared_ptr<SDL_Texture> _texture,
            const int _sheetX,
            const int _sheetY,
            const int _sheetW,
            const int _sheetH);

    const int getScreenX() const;
    const int getScreenY() const;
    const std::string getName() const;
    std::shared_ptr<SDL_Texture> getSheetTexture();
    const int getSheetX() const;
    const int getSheetY() const;
    const int getSheetW() const;
    const int getSheetH() const;

    static tile_pool generateTilesFrom(spritesheet_pool& spritesheetPool);

private:
    std::shared_ptr<SDL_Texture> sheetTexture;
    std::string name;
    int screenX;
    int screenY;
    int sheetX;
    int sheetY;
    int sheetW;
    int sheetH;
};

#endif
