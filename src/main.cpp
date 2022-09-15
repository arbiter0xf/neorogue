#include <boost/json/src.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

#include <string>
#include <iostream>
#include <fstream>
#include <array>

#include "Log.hpp"
#include "Sdlw.hpp"
#include "Tile.hpp"
#include "TextureSpritesheet.hpp"

const int TILE_HEIGHT = 32;
const int TILE_WIDTH = 32;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_TILES = (SCREEN_WIDTH / TILE_WIDTH) * (SCREEN_HEIGHT / TILE_HEIGHT) + 1; // +1 for null termination
const int TILE_POOL_SIZE = 3137; // TODO write a script for getting this and
                                 // pass by using -DTILE_POOL_SIZE=$(script)
const int TEXTURE_POOL_SIZE = 98; // TODO write a script for getting this and
                                  // pass by using -TEXUTRE_POOL_SIZE=$(script)

const char PROGRAM_NAME[] = "Rogue Forever";

using screen_tiles = std::array<Tile*, SCREEN_TILES>;
using tile_pool = std::array<Tile, TILE_POOL_SIZE>;
using texture_pool = std::array<SDL_Texture*, TEXTURE_POOL_SIZE>;

#if 0
void mainLoop(void) {
}
#endif

void readJsonFromFile(std::string filename, boost::json::value& jsonValue)
{
    std::ifstream jsonStream;
    std::string line;
    char buffer[4096] = {0};
    std::size_t readAmount = 0;
    boost::json::stream_parser jsonStreamParser;
    boost::json::error_code ec;

    printf(INFO "Reading JSON from file: %s\n", filename.c_str());

    jsonStream.open(filename, std::ifstream::in);
    if (jsonStream.fail()) {
        throw std::runtime_error(ERR "Failed to open json file: " + filename);
    }

    do {
        jsonStream.read(buffer, sizeof(buffer));
        readAmount = jsonStream.gcount();
        std::cout << "Successfully read " << readAmount << " characters" << "\n";
        jsonStreamParser.write(buffer, readAmount, ec);
        if (ec) {
            throw std::runtime_error(ERR "Stream parser write failed");
        }
    } while(!jsonStream.eof());

    jsonStreamParser.finish(ec);
    if (ec) {
        throw std::runtime_error(ERR "Stream parser finish failed");
    }

    jsonStream.clear();
    jsonStream.close();
    if (jsonStream.fail()) {
        throw std::runtime_error(ERR "Failed to close json file: " + filename);
    }

    jsonValue = jsonStreamParser.release();
}

// {"frames": {
//
//     "altars/dngn_altar.png":
//     {
//         "frame": {"x":35,"y":37,"w":32,"h":31},
//         "rotated": true,
//         "trimmed": true,
//         "spriteSourceSize": {"x":0,"y":1,"w":32,"h":31},
//         "sourceSize": {"w":32,"h":32}
//     },
//     "altars/dngn_altar_beogh.png":
//     {
/*
 * \exception Throws std::runtime_error on failure
 */
void texturepackerJsonGetFrameObject(
        std::string frameKey,
        const boost::json::value& jsonValue,
        boost::json::object& frameObjectOut)
{
    if (boost::json::kind::object != jsonValue.kind()) {
        throw std::runtime_error("JSON value top level is not an object");
    }

    auto const& topObj = jsonValue.get_object();
    auto iter = topObj.begin();
    boost::json::value framesValue = iter->value();

    auto frameIter = framesValue.get_object().find(frameKey);
    std::cout << DBG << "frameIter->key() is: " << frameIter->key() << "\n";
    std::cout << DBG << "frameIter->value() is: " << frameIter->value() << "\n";

    boost::json::value frameValue = frameIter->value();

    if (boost::json::kind::object != frameValue.kind()) {
        throw std::runtime_error("Frame JSON value is not an object");
    }

    frameObjectOut = frameValue.get_object();
}

boost::json::value jsonGetValueWithKey(
        const char* key,
        const boost::json::object& frameObject)
{
    auto frameObjectIter = frameObject.find(key);

#if DEBUG
    std::cout << DBG << "frameObjectIter->key() is: " << frameObjectIter->key() << "\n";
    std::cout << DBG << "frameObjectIter->value() is: " << frameObjectIter->value() << "\n";
#endif

    return frameObjectIter->value();
}

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

screen_tiles fillScreenTiles(tile_pool& tilePool)
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
tile_pool generateTilesFrom(
        Sdlw& sdlw,
        std::string imagePathSpritesheet,
        std::string dataPathSpritesheet)
{
    int ret = -1;

    boost::json::value tPackerJsonValue;
    boost::json::object tPackerFramesObj;
    tile_pool tilePool;

    SDL_Texture* textureSpritesheet = NULL;

    readJsonFromFile(dataPathSpritesheet, tPackerJsonValue);

    // TODO deallocate spritesheet
    //
    // Proposal: Add GraphicsPool that has a TexturePool
    textureSpritesheet = sdlw.imgLoadTexture(imagePathSpritesheet);
    if (NULL == textureSpritesheet) {
        throw std::runtime_error("Failed to load texture" + imagePathSpritesheet);
    }

    if (boost::json::kind::object != tPackerJsonValue.kind()) {
        throw std::runtime_error(ERR "JSON value top level is not an object");
    }

    auto const& topObj = tPackerJsonValue.get_object();
    if (topObj.empty()) {
        throw std::runtime_error("Empty top level JSON object");
    }

    auto iter = topObj.begin();
    tPackerFramesObj = iter->value().get_object();

    // TODO See also:
    // https://en.cppreference.com/w/cpp/algorithm/generate
    std::transform(
            tPackerFramesObj.cbegin(),
            tPackerFramesObj.cend(),
            tilePool.begin(),
            [&](const auto frameObj) {
                const auto& [tileName, tileConfig] = frameObj;
                const auto tileConfigObj = tileConfig.as_object();
                const auto tileConfigFrameObj = jsonGetValueWithKey("frame", tileConfigObj).as_object();
                return Tile(
                        -1,
                        -1,
                        tileName,
                        textureSpritesheet,
                        jsonGetValueWithKey("x", tileConfigFrameObj).as_int64(),
                        jsonGetValueWithKey("y", tileConfigFrameObj).as_int64(),
                        jsonGetValueWithKey("w", tileConfigFrameObj).as_int64(),
                        jsonGetValueWithKey("h", tileConfigFrameObj).as_int64());
            });

    return tilePool;
}

/*
 * \exception throws std::runtime_error on failure
 */
void initRendering(Sdlw& sdlw)
{
    int ret = -1;
    std::string msg = "";

    const int imgFlags = IMG_INIT_PNG;
    const int renderingDriver = -1; // -1 initializes the first driver
                                    // supporting requested flags

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
    const std::string assetsPrefix = "assets/";
    const std::string imagePathWallStoneGray1 =
        assetsPrefix + "dc-dngn/wall/stone_gray1.png";
    const std::string imagePathDngnSpritesheet =
        assetsPrefix + "spritesheets/dc-dngn.png";
    const std::string dataPathDngnSpritesheet =
        assetsPrefix + "spritesheets/dc-dngn.json";

    SDL_Rect srcRect = {0, 0, 0, 0};
    SDL_Rect dstRect = {0, 0, 0, 0};

    int err = -1;
    int ret = -1;
    bool quitEventReceived = false;

    Sdlw sdlw = Sdlw();

    SDL_Event event;

    tile_pool tilePool;
    texture_pool texturePool;

    screen_tiles screenTiles = { 0 };

    try {
        initRendering(sdlw);
    } catch(std::exception& e) {
        std::cerr << "Exception while initializing rendering: " << e.what();
        return 1;
    }

    try {
        // TODO eventually generateTilesFrom(texturePool);
        tilePool = generateTilesFrom(
                sdlw,
                imagePathDngnSpritesheet,
                dataPathDngnSpritesheet);
    } catch (std::exception const& e) {
        std::cerr << ERR << "Exception while generating tiles from spritesheets"
            << e.what() << "\n";
        sdlw.destroy();
        return -1;
    }

    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        sdlw.renderClear();

        screenTiles = fillScreenTiles(tilePool);

        renderScreenTiles(sdlw, screenTiles);

        sdlw.renderPresent();
    }

#if 0
    mainLoop();
#endif

    sdlw.destroy();

    return 0;
}
