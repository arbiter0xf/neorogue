#ifdef __linux__
#include <SDL2/SDL_image.h>
#endif

#ifdef _WIN32
#include <SDL2/SDL_image.h>
#endif

#include <iostream>
#include <string>

#include "Constants.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"

/*
 * About singleton and static variable lifetime
 * https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
 */
Sdlw Sdlw::sharedInstance = Sdlw();

#if 0
// TODO: Remove this reference code when in use elsewhere

Sdlw::Sdlw(Sdlw&& other)
    : mainWindow(std::exchange(other.mainWindow, nullptr))
    , mainRenderer(std::exchange(other.mainRenderer, nullptr))
{
}

Sdlw& Sdlw::operator=(Sdlw&& other)
{
    std::swap(mainWindow, other.mainWindow);
    std::swap(mainRenderer, other.mainRenderer);
    return *this;
}
#endif

Sdlw::~Sdlw()
{
    destroy();
}

Sdlw& Sdlw::getReference()
{
    return sharedInstance;
}

/*
 * \exception throws std::runtime_error on failure
 */
void Sdlw::initRendering(void)
{
    std::string msg = "";

    const int imgFlags = IMG_INIT_PNG;
    const int renderingDriver = -1; // -1 initializes the first driver
                                    // supporting requested flags

    Sdlw& sdlw = Sdlw::getReference();

    sdlw.init(SDL_INIT_VIDEO);

    sdlw.setHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    sdlw.imgInit(imgFlags);

    sdlw.createMainWindow(
            g_constants::PROGRAM_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            g_constants::SCREEN_WIDTH,
            g_constants::SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    sdlw.createMainRenderer(
            renderingDriver,
            SDL_RENDERER_ACCELERATED);

    sdlw.setRenderDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
}

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
        std::shared_ptr<SDL_Texture> texture,
        const SDL_Rect* srcrect,
        const SDL_Rect* dstrect)
{
    return SDL_RenderCopy(mainRenderer, texture.get(), srcrect, dstrect);
}

std::shared_ptr<SDL_Texture> Sdlw::imgLoadTextureShared(std::string file)
{
    std::shared_ptr<SDL_Texture> texture = NULL;
    std::string msg = "";

    texture = std::shared_ptr<SDL_Texture>(
            IMG_LoadTexture(mainRenderer, file.c_str()),
            [](SDL_Texture* p) {
                SDL_DestroyTexture(p);
            });
    if (NULL == texture.get()) {
        msg = "Failed to load texture: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }

    return texture;
}

void Sdlw::destroyTexture(std::shared_ptr<SDL_Texture> texture)
{
    SDL_DestroyTexture(texture.get());
}
