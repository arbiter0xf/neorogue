#ifndef SPRITESHEET_HPP_DEFINED
#define SPRITESHEET_HPP_DEFINED

#ifdef linux
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#include <SDL.h>
#endif

#include <memory>
#include <string>

#include "Json.hpp"
#include "Map.hpp"

const int SPRITESHEET_POOL_SIZE = 8;

class Spritesheet;

using spritesheet_pool = std::vector<Spritesheet>;

class Spritesheet {
public:
    Spritesheet() = default;
    ~Spritesheet() = default;
    Spritesheet(std::string _name, Map& map);

    std::string getName(void) const;
    std::shared_ptr<SDL_Texture> getTexture(void);
    const boost::json::value& getJson(void);
    int getTiledFirstgid(void) const;

    static const std::array<std::string, SPRITESHEET_POOL_SIZE> spritesheetNames;

private:
    std::string name;
    std::shared_ptr<SDL_Texture> texture;
    boost::json::value jsonValue;
    int tiledFirstgid;

    void loadTexture(std::string pathImage);
    void loadJson(std::string pathJson);
    void fetchFirstgid(Map& map);
};

#endif
