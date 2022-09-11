#ifndef TILE_HPP_DEFINED
#define TILE_HPP_DEFINED

#include <SDL2/SDL.h>

class Tile {
public:
    Tile() = delete;
    ~Tile();
    Tile(
            SDL_Texture* _texture,
            const int _x,
            const int _y,
            const int _w,
            const int _h);

    SDL_Texture* getSheetTexture();
    const int getX() const;
    const int getY() const;
    const int getW() const;
    const int getH() const;

private:
    SDL_Texture* sheetTexture;
    int x;
    int y;
    int w;
    int h;
};

#endif
