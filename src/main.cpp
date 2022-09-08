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
    const std::string assets_prefix = "assets/";
    const std::string image_path_wall_stone_gray1 =
        assets_prefix + "dc-dngn/wall/stone_gray1.png";

    const int imgFlags = IMG_INIT_PNG;

    int ret = -1;
    bool quitEventReceived = false;

    SDL_Window* mainWindow = NULL;
    SDL_Surface* mainWindowSurface = NULL;
    SDL_Surface* temporarySurface = NULL;
    SDL_Surface* imageSurface = NULL;
    SDL_Event event;

    ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        goto error_exit;
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

    mainWindowSurface = SDL_GetWindowSurface(mainWindow);
    if (NULL == mainWindowSurface) {
        printf("Failed to get window surface: %s\n", SDL_GetError());
        goto error_exit;
    }

    printf("Loading asset: %s\n", image_path_wall_stone_gray1.c_str());
    temporarySurface = IMG_Load(image_path_wall_stone_gray1.c_str());
    if (NULL == temporarySurface) {
        printf("Unable to load asset: %s Error: %s",
                image_path_wall_stone_gray1.c_str(),
                IMG_GetError());
        goto error_exit;
    }

    imageSurface = SDL_ConvertSurface(
            temporarySurface,
            mainWindowSurface->format,
            0);
    if (NULL == imageSurface) {
        printf("Failed to convert surface to format of main windows surface: %s\n", SDL_GetError());
        goto error_exit;
    }

    SDL_FreeSurface(temporarySurface);
    temporarySurface = NULL;

    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        SDL_BlitSurface(imageSurface, NULL, mainWindowSurface, NULL);

        SDL_UpdateWindowSurface(mainWindow);
    }

#if 0
    mainLoop();
#endif

    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;

    SDL_DestroyWindow(mainWindow);
    mainWindow = NULL;

    IMG_Quit();
    SDL_Quit();

    return 0;

error_exit:

    if (NULL != imageSurface) {
        SDL_FreeSurface(imageSurface);
        imageSurface = NULL;
    }

    if (NULL != mainWindow) {
        SDL_DestroyWindow(mainWindow);
        mainWindow = NULL;
    }

    IMG_Quit();
    SDL_Quit();

    return -1;
}
