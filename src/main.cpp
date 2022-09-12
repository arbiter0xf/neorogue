#include <boost/json/src.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

#include <string>
#include <iostream>
#include <fstream>
#include <array>

#include "Tile.hpp"

const int TILE_HEIGHT = 32;
const int TILE_WIDTH = 32;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_TILES = (SCREEN_WIDTH / TILE_WIDTH) * (SCREEN_HEIGHT / TILE_HEIGHT) + 1; // +1 for null termination
const int TILE_POOL_SIZE = 3137; // TODO write a script for getting this and
                                 // pass by using -DTILE_POOL_SIZE=$(script)

const char PROGRAM_NAME[] = "Rogue Forever";

#define WARN "[W] "
#define INFO "[I] "
#define ERR "[E] "
#define DBG "[D] "

#define DEBUG 1

using screen_tiles = std::array<Tile*, SCREEN_TILES>;
using tile_pool = std::array<Tile, TILE_POOL_SIZE>;

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

void texturepackerJsonGetValueWithKey(
        const char* key,
        const boost::json::object& frameObject,
        boost::json::value& frameObjectFrame)
{
    // auto frameObjectIter = frameObject.find("frame");
    auto frameObjectIter = frameObject.find(key);

    std::cout << DBG << "frameObjectIter->key() is: " << frameObjectIter->key() << "\n";
    std::cout << DBG << "frameObjectIter->value() is: " << frameObjectIter->value() << "\n";

    frameObjectFrame = frameObjectIter->value();
}

void renderTile(
        SDL_Renderer* renderer,
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

    SDL_RenderCopy(renderer, tile->getSheetTexture(), &srcRect, &dstRect);
}

screen_tiles fillScreenTiles(tile_pool& tilePool)
{
    return { &tilePool[0] };
}

void renderScreenTiles(
        SDL_Renderer* renderer,
        screen_tiles screenTiles)
{
    for (Tile* tile : screenTiles) {
        if (NULL == tile) {
            // screenTiles is assumed to contain valid tiles starting from
            // index 0 and continuing until NULL ptr is met or end of
            // screenTiles
            break;
        }

        renderTile(renderer, tile);
    }
}

/*
 * \exception May throw std::exception
 */
tile_pool generateTilesFrom(
        SDL_Renderer* renderer,
        std::string imagePathSpritesheet,
        std::string dataPathSpritesheet)
{
    int ret = -1;

    boost::json::value tPackerJsonValue;
    boost::json::value tPackerJsonTemp;
    boost::json::object tPackerFramesObj;
    int tileSheetX = -1;
    int tileSheetY = -1;
    int tileSheetW = -1;
    int tileSheetH = -1;
    Tile tileTemp;
    tile_pool tilePool;

    SDL_Texture* textureSpritesheet = NULL;

    readJsonFromFile(dataPathSpritesheet, tPackerJsonValue);

    // TODO deallocate spritesheet
    //
    // Proposal: Add GraphicsPool that has a TexturePool
    textureSpritesheet = IMG_LoadTexture(
            renderer,
            imagePathSpritesheet.c_str());
    if (NULL == textureSpritesheet) {
        throw std::runtime_error("Failed to load texture" + imagePathSpritesheet);
    }

    if (boost::json::kind::object != tPackerJsonValue.kind()) {
        printf(ERR "JSON value top level is not an object\n");
    }

    auto const& topObj = tPackerJsonValue.get_object();
    if (topObj.empty()) {
        throw std::runtime_error("Empty top level JSON object");
    }

    auto iter = topObj.begin();
    tPackerFramesObj = iter->value().get_object();
    iter = tPackerFramesObj.begin();
    auto iterEnd = tPackerFramesObj.end();

    auto tilePoolIter = tilePool.begin();
    for (const auto& [tileName, tileConfig] : tPackerFramesObj) {
        if (tilePoolIter == tilePool.end()) {
            throw std::runtime_error("Reached end of tilePool");
        }

        const auto tileConfigObj = tileConfig.as_object();
        texturepackerJsonGetValueWithKey("frame", tileConfigObj, tPackerJsonTemp);
        const auto tileConfigFrameObj = tPackerJsonTemp.as_object();

#if DEBUG
        std::cout << DBG << "Constructing tile for tile pool:" << "\n";
        std::cout << DBG << "tileName is: " << tileName << "\n";
        std::cout << DBG << "tileConfig is: " << tileConfig << "\n";
#endif

        texturepackerJsonGetValueWithKey("x", tileConfigFrameObj, tPackerJsonTemp);
        tileSheetX = tPackerJsonTemp.as_int64();
        texturepackerJsonGetValueWithKey("y", tileConfigFrameObj, tPackerJsonTemp);
        tileSheetY = tPackerJsonTemp.as_int64();
        texturepackerJsonGetValueWithKey("w", tileConfigFrameObj, tPackerJsonTemp);
        tileSheetW = tPackerJsonTemp.as_int64();
        texturepackerJsonGetValueWithKey("h", tileConfigFrameObj, tPackerJsonTemp);
        tileSheetH = tPackerJsonTemp.as_int64();

        // TODO use std::generate & std::transform
        // https://en.cppreference.com/w/cpp/algorithm/generate
        // https://en.cppreference.com/w/cpp/algorithm/transform
        // std::transform(tPackerFramesObj.cbegin(), tPackerFramesObj.cend(), tilePool.begin(), [](auto frame) { ... return Tile{...};});
        *tilePoolIter = Tile(
                -1,
                -1,
                tileName,
                textureSpritesheet,
                tileSheetX,
                tileSheetY,
                tileSheetW,
                tileSheetH);

        if (tilePool.begin() == tilePoolIter) {
            // Print only once
#if DEBUG
            std::cout << DBG << "Constructed a tile with:" << "\n";
            std::cout << DBG << "sheetX: " << (*tilePoolIter).getSheetX() << "\n";
            std::cout << DBG << "sheetY: " << (*tilePoolIter).getSheetY() << "\n";
            std::cout << DBG << "sheetW: " << (*tilePoolIter).getSheetW() << "\n";
            std::cout << DBG << "sheetH: " << (*tilePoolIter).getSheetH() << "\n";
            std::cout << DBG << "name: " << (*tilePoolIter).getName() << "\n";
#endif
        }

        tilePoolIter++;
    }

    return tilePool;
}

void cleanup(void)
{
    printf(INFO "Cleanup placeholder running");
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

    const int imgFlags = IMG_INIT_PNG;

    const int renderingDriver = -1; // -1 initializes the first driver
                                    // supporting requested flags

    boost::json::value tPackerJsonValue;
    boost::json::value tPackerJsonTemp;
    boost::json::object frameObject;
    boost::json::object frameAltarObject;
    boost::json::object frameAltarObjectFrame;
    boost::json::object frameStoneGrayObject;
    boost::json::object frameStoneGrayObjectFrame;
    boost::json::object frameGateObject;
    boost::json::object frameGateObjectFrame;

    SDL_Rect srcRect = {0, 0, 0, 0};
    SDL_Rect dstRect = {0, 0, 0, 0};

    int err = -1;
    int ret = -1;
    bool quitEventReceived = false;

    SDL_Window* mainWindow = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    tile_pool tilePool;

    std::array<Tile*, SCREEN_TILES> screenTiles = { 0 };

    try {
        readJsonFromFile(dataPathDngnSpritesheet, tPackerJsonValue);
    } catch(std::exception const& e) {
        std::cerr << ERR << "Exception while reading JSON from file: "
            << e.what() << "\n";
        goto error_exit;
    }

    ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) {
        printf(ERR "Failed to initialize SDL: %s\n", SDL_GetError());
        goto error_exit;
    }

    ret = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    if (ret < 0) {
        printf(WARN "Warning: Linear texture filtering not enabled!");
    }

    ret = IMG_Init(imgFlags);
    if (0 == ret) {
        printf(ERR "Failed to initialize SDL_image: %s\n", IMG_GetError());
        goto error_exit;
    }

    mainWindow = SDL_CreateWindow(
            PROGRAM_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
    if (NULL == mainWindow) {
        printf(ERR "Failed to create window: %s\n", SDL_GetError());
        goto error_exit;
    }

    renderer = SDL_CreateRenderer(
            mainWindow,
            renderingDriver,
            SDL_RENDERER_ACCELERATED );
    if(NULL == renderer) {
        printf(ERR "Failed to create renderer. SDL Error: %s\n", SDL_GetError());
        goto error_exit;
    }

    ret = SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (ret < 0) {
        printf(ERR "Failed to set render draw color: %s\n", SDL_GetError());
        goto error_exit;
    }

    // TODO consider handling errors without gotos
    //
    // See lippincott functions:
    // https://isocpp.org/blog/2017/11/cpp-weekly-episode-91-using-lippincott-functions-jason-turner
    // https://cppsecrets.blogspot.com/
    texture = IMG_LoadTexture(renderer, imagePathWallStoneGray1.c_str());
    if (NULL == texture) {
        printf(ERR "Failed to load texture from file %s\n",
                imagePathWallStoneGray1.c_str());
        goto error_exit;
    }

    try {
        tilePool = generateTilesFrom(
                renderer,
                imagePathDngnSpritesheet,
                dataPathDngnSpritesheet);
    } catch (std::exception const& e) {
        std::cerr << ERR << "Exception while generating tiles from spritesheets"
            << e.what() << "\n";
        cleanup();
        return -1;
    }

    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        SDL_RenderClear(renderer);

        screenTiles = fillScreenTiles(tilePool);

        renderScreenTiles(renderer, screenTiles);

        SDL_RenderPresent(renderer);
    }

#if 0
    mainLoop();
#endif

    SDL_DestroyTexture(texture);
    texture = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(mainWindow);
    mainWindow = NULL;

    IMG_Quit();
    SDL_Quit();

    return 0;

error_exit:

    if (NULL != texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }

    if (NULL != renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (NULL != mainWindow) {
        SDL_DestroyWindow(mainWindow);
        mainWindow = NULL;
    }

    IMG_Quit();
    SDL_Quit();

    return -1;
}
