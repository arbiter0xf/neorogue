// Header-only with multiple translation units
// https://www.boost.org/doc/libs/1_69_0/libs/test/doc/html/boost_test/adv_scenarios/single_header_customizations/multiple_translation_units.html
// Must to include boost/json/src.hpp in only one source file and other files
// must then include boost/json.hpp.
#include <boost/json/src.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

#include <array>
#include <iostream>
#include <string>

#include "Assets.hpp"
#include "Constants.hpp"
#include "GraphicsUtil.hpp"
#include "Json.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"
#include "Tile.hpp"

using screen_tiles = std::array<std::array<Tile*, g_constants::TILES_HORIZONTAL>, g_constants::TILES_VERTICAL>;
using texture_pool = std::array<SDL_Texture*, g_constants::TEXTURE_POOL_SIZE>;

#if 0
void mainLoop(void) {
}
#endif

void renderTile(
        Sdlw& sdlw,
        Tile* tile)
{
    SDL_Rect srcRect = {
        tile->getSheetX(),
        tile->getSheetY(),
        tile->getSheetW(),
        tile->getSheetH(),
    };
    SDL_Rect dstRect = {
        tile->getScreenX(),
        tile->getScreenY(),
        tile->getSheetW(),
        tile->getSheetH()
    };

    sdlw.renderCopy(tile->getSheetTexture(), &srcRect, &dstRect);
}

void fillScreenTiles(
        tile_pool& tilePool,
        Level& level,
        tile_id_map& tileIdMap,
        int cameraX,
        int cameraY,
        screen_tiles& screenTiles)
{
#if 0
    screenX = cameraX - g_constants::TILES_HORIZONTAL / 2;
    screenY = cameraY - g_constants::TILES_VERTICAL / 2;
#endif
    screenTiles[0][0] = &tilePool[tileIdMap[level.getTileId(0, 0)]];
}

void renderScreenTiles(
        Sdlw& sdlw,
        const screen_tiles& screenTiles)
{
    for (const auto tilesRow : screenTiles) {
        for (Tile* tile : tilesRow) {
            if (NULL == tile) {
                continue;
            }

            renderTile(sdlw, tile);
        }
    }
}

void game(void)
{
    int err = -1;
    int ret = -1;
    int cameraX = 10;
    int cameraY = 7;
    bool quitEventReceived = false;

    Sdlw& sdlw = Sdlw::getReference();

    SDL_Event event;

    tile_pool tilePool;
    tile_id_map tileIdMap;
    texture_pool texturePool;
    spritesheet_pool spritesheetPool;

    screen_tiles screenTiles;

    for (auto tilesRow : screenTiles) {
        tilesRow.fill(0);
    }

    Log::i("Loading test level");
    Level testLevel1 = Level("levels/test_level1.txt");

    Log::i("Initializing rendering");
    try {
        Sdlw::initRendering();
    } catch(std::exception& e) {
        std::cerr << "Exception while initializing rendering: " << e.what();
        throw e;
    }

    Log::i("Loading spritesheets");
    GraphicsUtil::loadSpritesheets(spritesheetPool);

    Log::i("Generating tiles");
    try {
        GraphicsUtil::generateTiles(spritesheetPool, tilePool);
    } catch (std::exception const& e) {
        std::cerr << ERR << "Exception while generating tiles from spritesheets: "
            << e.what() << "\n";
        throw e;
    }

    Log::i("Generating tile ID map");
    GraphicsUtil::generateTileIdMap(spritesheetPool, tileIdMap);
    GraphicsUtil::generateTileIdMapFile(tileIdMap);

    Log::i("Entering main loop");
    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        sdlw.renderClear();

        fillScreenTiles(tilePool,
			testLevel1,
			tileIdMap,
			cameraX,
			cameraY,
			screenTiles);

        renderScreenTiles(sdlw, screenTiles);

        sdlw.renderPresent();
    }

#if 0
    mainLoop();
#endif

    sdlw.destroy();
}

int main(void)
{
    try {
        game();
    } catch(std::exception const& e) {
        std::string msg = "Terminating due to unhandled exception: ";
        msg += e.what();
        Log::e(msg);
    }
}
