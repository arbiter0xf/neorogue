#ifndef TILE_HPP_DEFINED
#define TILE_HPP_DEFINED

#include <SDL2/SDL.h>

class Tile {
public:
    Tile() = delete;
    ~Tile();
    Tile(
            const int _screenX,
            const int _screenY,
            SDL_Texture* _texture,
            const int _sheetX,
            const int _sheetY,
            const int _sheetW,
            const int _sheetH);

    const int getScreenX() const;
    const int getScreenY() const;
    SDL_Texture* getSheetTexture();
    const int getSheetX() const;
    const int getSheetY() const;
    const int getSheetW() const;
    const int getSheetH() const;

private:
    SDL_Texture* sheetTexture;
    int screenX;
    int screenY;
    int sheetX;
    int sheetY;
    int sheetW;
    int sheetH;
};

#endif
