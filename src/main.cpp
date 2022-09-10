#include <boost/json/src.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const char PROGRAM_NAME[] = "Rogue Forever";

#if 0
void mainLoop(void) {
}
#endif

int main(void) {
    const std::string assetsPrefix = "assets/";
    const std::string imagePathWallStoneGray1 =
        assetsPrefix + "dc-dngn/wall/stone_gray1.png";
    const std::string imagePathDngnWallSpritesheet =
        assetsPrefix + "spritesheets/dc-dngn_wall.png";

    const int imgFlags = IMG_INIT_PNG;

    const int renderingDriver = -1; // -1 initializes the first driver
                                    // supporting requested flags

    SDL_Rect srcRect = {0, 0, 0, 0};
    SDL_Rect dstRect = {0, 0, 0, 0};

    int ret = -1;
    bool quitEventReceived = false;

    SDL_Window* mainWindow = NULL;
    SDL_Texture* texture = NULL;
    SDL_Texture* textureSpritesheet = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

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
            imagePathDngnWallSpritesheet.c_str());
    if (NULL == textureSpritesheet) {
        printf("Failed to load texture from file %s\n",
                imagePathDngnWallSpritesheet.c_str());
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
