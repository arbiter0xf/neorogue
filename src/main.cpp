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
#include "Json.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"
#include "Tile.hpp"

using screen_tiles = std::array<Tile*, g_constants::SCREEN_TILES>;
using tile_pool = std::array<Tile, g_constants::TILE_POOL_SIZE>;
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

screen_tiles fillScreenTiles(
        tile_pool& tilePool,
        Level& level,
        int cameraX,
        int cameraY)
{
    return { &tilePool[0] };
}

void renderScreenTiles(
        Sdlw& sdlw,
        screen_tiles screenTiles)
{
    for (Tile* tile : screenTiles) {
        if (NULL == tile) {
            // screenTiles is assumed to contain valid tiles starting from
            // index 0 and continuing until NULL ptr is met or end of
            // screenTiles
            break;
        }

        renderTile(sdlw, tile);
    }
}

/*
 * \exception May throw std::exception
 */
tile_pool generateTilesFrom(spritesheet_pool& spritesheetPool)
{
    int ret = -1;

    boost::json::object tPackerFramesObj;
    tile_pool tilePool;

    Sdlw& sdlw = Sdlw::getReference();

    auto tilePoolIter = tilePool.begin();

    for (Spritesheet& spritesheet : spritesheetPool) {
        boost::json::value tPackerJsonValue = spritesheet.getJson();
        std::shared_ptr<SDL_Texture> textureSpritesheet = spritesheet.getTexture();

        tPackerFramesObj = Json::getFirstInnerObject(tPackerJsonValue);

        // TODO See also:
        // https://en.cppreference.com/w/cpp/algorithm/generate
        std::transform(
                tPackerFramesObj.cbegin(),
                tPackerFramesObj.cend(),
                tilePoolIter,
                [&](const auto frameObj) {
                    const auto& [tileName, tileConfig] = frameObj;
                    const auto tileConfigObj = tileConfig.as_object();
                    const auto tileConfigFrameObj = Json::getValueWithKey("frame", tileConfigObj).as_object();

                    return Tile(
                            -1,
                            -1,
                            tileName,
                            textureSpritesheet,
                            Json::getValueWithKey("x", tileConfigFrameObj).as_int64(),
                            Json::getValueWithKey("y", tileConfigFrameObj).as_int64(),
                            Json::getValueWithKey("w", tileConfigFrameObj).as_int64(),
                            Json::getValueWithKey("h", tileConfigFrameObj).as_int64());
                });

        // Iterator seemed to point to where it pointed before calling
        // std::transform(). Walk to first uninitialized Tile in tilePool.
        while (NULL != tilePoolIter->getSheetTexture()) {
            if (tilePool.end() == tilePoolIter) {
                throw std::runtime_error("Reached end of tile pool when generating tiles");
            }

            tilePoolIter++;
        }
    }

    return tilePool;
}

int main(void)
{
    int err = -1;
    int ret = -1;
    int cameraX = 0;
    int cameraY = 0;
    bool quitEventReceived = false;

    Sdlw& sdlw = Sdlw::getReference();

    SDL_Event event;

    tile_pool tilePool;
    texture_pool texturePool;
    spritesheet_pool spritesheetPool;

    screen_tiles screenTiles = { 0 };

    Log::i("Loading test level");
    Level testLevel1 = Level("levels/test_level1.txt");

    Log::i("Initializing rendering");
    try {
        Sdlw::initRendering();
    } catch(std::exception& e) {
        std::cerr << "Exception while initializing rendering: " << e.what();
        return 1;
    }

    Log::i("Loading spritesheets");
    Spritesheet::loadSpritesheets(spritesheetPool);

    Log::i("Generating tiles");
    try {
        tilePool = generateTilesFrom(spritesheetPool);
    } catch (std::exception const& e) {
        std::cerr << ERR << "Exception while generating tiles from spritesheets: "
            << e.what() << "\n";
        return 1;
    }

    Log::i("Entering main loop");
    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        sdlw.renderClear();

        screenTiles = fillScreenTiles(tilePool, testLevel1, cameraX, cameraY);

        renderScreenTiles(sdlw, screenTiles);

        sdlw.renderPresent();
    }

#if 0
    mainLoop();
#endif

    sdlw.destroy();

    return 0;
}
