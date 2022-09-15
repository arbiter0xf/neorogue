#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include "Log.hpp"
#include "Sdlw.hpp"

void Sdlw::init(std::uint32_t flags)
{
    int ret = -1;
    std::string msg = "";

    ret = SDL_Init(flags);
    if (ret < 0) {
        msg = "Failed to initialize SDL: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

void Sdlw::destroy(void)
{
    if (NULL != mainRenderer) {
        SDL_DestroyRenderer(mainRenderer);
        mainRenderer = NULL;
    }

    if (NULL != mainWindow) {
        SDL_DestroyWindow(mainWindow);
        mainWindow = NULL;
    }

    IMG_Quit();
    SDL_Quit();
}

void Sdlw::setHint(std::string name, std::string value)
{
    int ret = -1;

    ret = SDL_SetHint(name.c_str(), value.c_str());
    if (ret < 0) {
        std::cout << WARN << "Setting SDL hint " << name << " failed";
    }
}

void Sdlw::imgInit(int flags)
{
    int ret = -1;
    std::string msg = "";

    ret = IMG_Init(flags);
    if (0 == ret) {
        msg = "Failed to initialize SDL_image: ";
        msg += IMG_GetError();
        throw std::runtime_error(msg);
    }
}

void Sdlw::createMainWindow(
        std::string title,
        int x,
        int y,
        int w,
        int h,
        std::uint32_t flags)
{
    std::string msg = "";

    mainWindow = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    if (NULL == mainWindow) {
        msg = "Failed to create window: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

void Sdlw::createMainRenderer(int index, std::uint32_t flags)
{
    std::string msg = "";

    mainRenderer = SDL_CreateRenderer(mainWindow, index, flags);
    if(NULL == mainRenderer) {
        msg = "Failed to create renderer. SDL Error: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

void Sdlw::setRenderDrawColor(
        std::uint8_t r,
        std::uint8_t g,
        std::uint8_t b,
        std::uint8_t a)
{
    int ret = -1;
    std::string msg = "";

    ret = SDL_SetRenderDrawColor(mainRenderer, r, g, b, a);
    if (ret < 0) {
        msg = "Failed to set render draw color: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

void Sdlw::renderClear(void)
{
    SDL_RenderClear(mainRenderer);
}

void Sdlw::renderPresent(void)
{
    SDL_RenderPresent(mainRenderer);
}

int Sdlw::renderCopy(
        SDL_Texture* texture,
        const SDL_Rect* srcrect,
        const SDL_Rect* dstrect)
{
    return SDL_RenderCopy(mainRenderer, texture, srcrect, dstrect);
}

SDL_Texture* Sdlw::imgLoadTexture(std::string file)
{
    return IMG_LoadTexture(mainRenderer, file.c_str());
}
