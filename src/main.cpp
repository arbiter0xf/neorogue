#include <boost/json/src.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

#include <string>
#include <iostream>
#include <fstream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const char PROGRAM_NAME[] = "Rogue Forever";

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

    printf("Reading JSON from file: %s\n", filename.c_str());

    jsonStream.open(filename, std::ifstream::in);
    if (jsonStream.fail()) {
        printf("Failed to open json file: %s\n", filename.c_str());
        return -1;
    }

    do {
        jsonStream.read(buffer, sizeof(buffer));
        readAmount = jsonStream.gcount();
        std::cout << "Successfully read " << readAmount << " characters" << "\n";
        jsonStreamParser.write(buffer, readAmount, ec);
        if (ec) {
            printf("Stream parser write failed");
            return -1;
        }
    } while(!jsonStream.eof());

    jsonStreamParser.finish(ec);
    if (ec) {
        printf("Stream parser finish failed");
        return -1;
    }

    jsonStream.clear();
    jsonStream.close();
    if (jsonStream.fail()) {
        printf("Failed to close json file: %s\n", filename.c_str());
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
int texturepackerJsonGetFilenames(boost::json::value const& jsonValue, std::string& filenames)
{
    filenames = "";

    if (boost::json::kind::object != jsonValue.kind()) {
        printf("JSON value top level is not an object\n");
        return -1;
    }

    auto const& topObj = jsonValue.get_object();

    if (topObj.empty()) {
        printf("Empty top level JSON object\n");
        return -1;
    }

    auto iter = topObj.begin();
    std::cout << "iter points to topObj.begin(): " << iter->key() << "\n";
    std::cout << "iter->key is: " << iter->key() << "\n";

    boost::json::value framesObj = iter->value();

    // iter is expected to point to "frames"
    if (boost::json::kind::object != framesObj.kind()) {
        printf("iter does not seem to point to \"frames\"\n");
        return -1;
    }

    iter = framesObj.get_object().begin();
    auto iterEnd = framesObj.get_object().end();
    std::cout << "iter points to framesObj.begin(): " << iter->key() << "\n";
    std::cout << "iter->key is: " << iter->key() << "\n";

    while (iter != iterEnd) {
        filenames += iter->key();
        filenames += "\n";
        iter++;
    }

    return 0;
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

    boost::json::value jsonValue;

    std::string dngnSpritesheetFilenames = "";

    SDL_Rect srcRect = {0, 0, 0, 0};
    SDL_Rect dstRect = {0, 0, 0, 0};

    int ret = -1;
    bool quitEventReceived = false;

    SDL_Window* mainWindow = NULL;
    SDL_Texture* texture = NULL;
    SDL_Texture* textureSpritesheet = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    try {
        ret = readJsonFromFile(dataPathDngnSpritesheet, jsonValue);
    } catch(std::exception const& e) {
        std::cerr << "Exception while reading JSON from file: "
            << e.what() << "\n";
        goto error_exit;
    }
    if (0 != ret) {
        printf("Failed to read JSON from file: %s\n",
                dataPathDngnSpritesheet.c_str());
        goto error_exit;
    }

    ret = texturepackerJsonGetFilenames(jsonValue, dngnSpritesheetFilenames);
    if (0 != ret) {
        printf("texturepackerJsonGetFilenames() failed");
        goto error_exit;
    }

    std::cout << "Found following filenames from JSON value:\n";
    std::cout << dngnSpritesheetFilenames << "\n";

    ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        goto error_exit;
    }

    ret = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    if (ret < 0) {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    ret = IMG_Init(imgFlags);
    if (0 == ret) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
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
        printf("Failed to create window: %s\n", SDL_GetError());
        goto error_exit;
    }

    renderer = SDL_CreateRenderer(
            mainWindow,
            renderingDriver,
            SDL_RENDERER_ACCELERATED );
    if(NULL == renderer) {
        printf("Failed to create renderer. SDL Error: %s\n", SDL_GetError());
        goto error_exit;
    }

    ret = SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (ret < 0) {
        printf("Failed to set render draw color: %s\n", SDL_GetError());
        goto error_exit;
    }

    texture = IMG_LoadTexture(renderer, imagePathWallStoneGray1.c_str());
    if (NULL == texture) {
        printf("Failed to load texture from file %s\n",
                imagePathWallStoneGray1.c_str());
        goto error_exit;
    }

    textureSpritesheet = IMG_LoadTexture(
            renderer,
            imagePathDngnSpritesheet.c_str());
    if (NULL == textureSpritesheet) {
        printf("Failed to load texture from file %s\n",
                imagePathDngnSpritesheet.c_str());
        goto error_exit;
    }

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
