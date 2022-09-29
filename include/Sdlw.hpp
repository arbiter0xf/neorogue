#ifndef SDLW_HPP_DEFINED
#define SDLW_HPP_DEFINED

#include <SDL2/SDL.h>
#include <cstdint>
#include <memory>

class Sdlw {
public:
    Sdlw(const Sdlw& other) = delete;
    Sdlw& operator=(const Sdlw& other) = delete;
    Sdlw(Sdlw&& other) = delete;
    Sdlw& operator=(Sdlw&& other) = delete;

    ~Sdlw();

    void init(std::uint32_t flags);
    void setHint(std::string name, std::string value);
    void imgInit(int flags);
    void createMainWindow(
            std::string title,
            int x,
            int y,
            int w,
            int h,
            std::uint32_t flags);
    void createMainRenderer(int index, std::uint32_t flags);
    void setRenderDrawColor(
            std::uint8_t r,
            std::uint8_t g,
            std::uint8_t b,
            std::uint8_t a);
    void destroy(void);
    void renderClear(void);
    void renderPresent(void);
    int renderCopy(
            std::shared_ptr<SDL_Texture> texture,
            const SDL_Rect* srcrect,
            const SDL_Rect* dstrect);
    std::shared_ptr<SDL_Texture> imgLoadTextureShared(std::string file);
    void destroyTexture(std::shared_ptr<SDL_Texture> texture);

    static Sdlw& getReference(void);
    static void initRendering(void);

private:
    Sdlw() = default;

    static Sdlw sharedInstance;

    SDL_Window* mainWindow;
    SDL_Renderer* mainRenderer;
};

#endif // SDLW_HPP_DEFINED
