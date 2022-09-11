#include <boost/json/src.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

#include <string>
#include <iostream>
#include <fstream>

#include "tile.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const char PROGRAM_NAME[] = "Rogue Forever";

#define WARN "[W] "
#define INFO "[I] "
#define ERR "[E] "
#define DBG "[D] "

#if 0
void mainLoop(void) {
}
#endif

int readJsonFromFile(std::string filename, boost::json::value& jsonValue)
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
        printf(ERR "Failed to open json file: %s\n", filename.c_str());
        return -1;
    }

    do {
        jsonStream.read(buffer, sizeof(buffer));
        readAmount = jsonStream.gcount();
        std::cout << "Successfully read " << readAmount << " characters" << "\n";
        jsonStreamParser.write(buffer, readAmount, ec);
        if (ec) {
            printf(ERR "Stream parser write failed");
            return -1;
        }
    } while(!jsonStream.eof());

    jsonStreamParser.finish(ec);
    if (ec) {
        printf(ERR "Stream parser finish failed");
        return -1;
    }

    jsonStream.clear();
    jsonStream.close();
    if (jsonStream.fail()) {
        printf(ERR "Failed to close json file: %s\n", filename.c_str());
        return -1;
    }

    jsonValue = jsonStreamParser.release();

    return 0;
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
        const char* frameKey,
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

void renderTileFromSpritesheet(
        SDL_Renderer* renderer,
        Tile& tile,
        int destX,
        int destY)
{
    SDL_Rect srcRect = {
        tile.getX(),
        tile.getY(),
        tile.getW(),
        tile.getH(),
    };
    SDL_Rect dstRect = {
        destX,
        destY,
        tile.getW(),
        tile.getH()
    };

    SDL_RenderCopy(renderer, tile.getSheetTexture(), &srcRect, &dstRect);
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

    SDL_Rect srcRect = {0, 0, 0, 0};
    SDL_Rect dstRect = {0, 0, 0, 0};

    int err = -1;
    int ret = -1;
    bool quitEventReceived = false;

    SDL_Texture* tileAltarSpritesheet = NULL;
    const char* tileAltarPath = "altars/dngn_altar.png";
    int tileAltarX = -1;
    int tileAltarY = -1;
    int tileAltarW = -1;
    int tileAltarH = -1;

    SDL_Window* mainWindow = NULL;
    SDL_Texture* texture = NULL;
    SDL_Texture* textureSpritesheet = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    try {
        ret = readJsonFromFile(dataPathDngnSpritesheet, tPackerJsonValue);
    } catch(std::exception const& e) {
        std::cerr << ERR << "Exception while reading JSON from file: "
            << e.what() << "\n";
        goto error_exit;
    }
    if (0 != ret) {
        printf(ERR "Failed to read JSON from file: %s\n",
                dataPathDngnSpritesheet.c_str());
        goto error_exit;
    }

    texturepackerJsonGetFrameObject(tileAltarPath, tPackerJsonValue, frameAltarObject);
    texturepackerJsonGetValueWithKey("frame", frameAltarObject, tPackerJsonTemp);
    frameAltarObjectFrame = tPackerJsonTemp.get_object();

    try {
        texturepackerJsonGetValueWithKey("x", frameAltarObjectFrame, tPackerJsonTemp);
        tileAltarX = tPackerJsonTemp.as_int64();
        texturepackerJsonGetValueWithKey("y", frameAltarObjectFrame, tPackerJsonTemp);
        tileAltarY = tPackerJsonTemp.as_int64();
        texturepackerJsonGetValueWithKey("w", frameAltarObjectFrame, tPackerJsonTemp);
        tileAltarW = tPackerJsonTemp.as_int64();
        texturepackerJsonGetValueWithKey("h", frameAltarObjectFrame, tPackerJsonTemp);
        tileAltarH = tPackerJsonTemp.as_int64();
    } catch(std::exception const& e) {
        std::cerr << ERR << "Exception while converting json value to uint64"
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

    texture = IMG_LoadTexture(renderer, imagePathWallStoneGray1.c_str());
    if (NULL == texture) {
        printf(ERR "Failed to load texture from file %s\n",
                imagePathWallStoneGray1.c_str());
        goto error_exit;
    }

    textureSpritesheet = IMG_LoadTexture(
            renderer,
            imagePathDngnSpritesheet.c_str());
    if (NULL == textureSpritesheet) {
        printf(ERR "Failed to load texture from file %s\n",
                imagePathDngnSpritesheet.c_str());
        goto error_exit;
    }

    tileAltarSpritesheet = textureSpritesheet;

    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        SDL_RenderClear(renderer);

        srcRect = {0, 0, 32, 32};
        dstRect = {0, 0, 32, 32};
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);

        srcRect.x = 409;
        srcRect.y = 137;
        srcRect.w = 32;
        srcRect.h = 32;
        dstRect = {32, 32, 32, 32};
        SDL_RenderCopy(renderer, textureSpritesheet, &srcRect, &dstRect);

        Tile testTile(
                tileAltarSpritesheet,
                tileAltarX,
                tileAltarY,
                tileAltarW,
                tileAltarH);
        renderTileFromSpritesheet(
                renderer,
                testTile,
                64,
                64);

        SDL_RenderPresent(renderer);
    }

#if 0
    mainLoop();
#endif

    SDL_DestroyTexture(texture);
    texture = NULL;

    SDL_DestroyTexture(textureSpritesheet);
    texture = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(mainWindow);
    mainWindow = NULL;

    IMG_Quit();
    SDL_Quit();

    return 0;

error_exit:

    if (NULL != textureSpritesheet) {
        SDL_DestroyTexture(textureSpritesheet);
        texture = NULL;
    }

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
