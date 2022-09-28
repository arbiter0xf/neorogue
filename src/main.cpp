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
#include "Json.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"
#include "Tile.hpp"

const int TILE_HEIGHT = 32;
const int TILE_WIDTH = 32;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_TILES = (SCREEN_WIDTH / TILE_WIDTH) * (SCREEN_HEIGHT / TILE_HEIGHT) + 1; // +1 for null termination
const int TILE_POOL_SIZE = 3137; // TODO write a script for getting this and
                                 // pass by using -DTILE_POOL_SIZE=$(script)
const int TEXTURE_POOL_SIZE = 98; // TODO write a script for getting this and
                                  // pass by using -DTEXUTRE_POOL_SIZE=$(script)

const char PROGRAM_NAME[] = "Rogue Forever";

using screen_tiles = std::array<Tile*, SCREEN_TILES>;
using tile_pool = std::array<Tile, TILE_POOL_SIZE>;
using texture_pool = std::array<SDL_Texture*, TEXTURE_POOL_SIZE>;

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
tile_pool generateTilesFrom(spritesheet_pool spritesheetPool)
{
    int ret = -1;

    boost::json::object tPackerFramesObj;
    tile_pool tilePool;

    Sdlw& sdlw = Sdlw::getReference();

    auto tilePoolIter = tilePool.begin();

    for (Spritesheet spritesheet : spritesheetPool) {
        boost::json::value tPackerJsonValue = spritesheet.getJson();
        SDL_Texture* textureSpritesheet = spritesheet.getTexture();

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

/*
 * \exception throws std::runtime_error on failure
 */
void initRendering(void)
{
    int ret = -1;
    std::string msg = "";

    const int imgFlags = IMG_INIT_PNG;
    const int renderingDriver = -1; // -1 initializes the first driver
                                    // supporting requested flags

    Sdlw& sdlw = Sdlw::getReference();

    sdlw.init(SDL_INIT_VIDEO);

    sdlw.setHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    sdlw.imgInit(imgFlags);

    sdlw.createMainWindow(
            PROGRAM_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    sdlw.createMainRenderer(
            renderingDriver,
            SDL_RENDERER_ACCELERATED);

    sdlw.setRenderDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
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
        initRendering();
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
        sdlw.destroy();
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
