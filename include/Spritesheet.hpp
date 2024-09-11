#ifndef SPRITESHEET_HPP_DEFINED
#define SPRITESHEET_HPP_DEFINED

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#include <SDL2/SDL.h>
// #include <SDL.h>
#endif

#include <memory>
#include <string>
#include <vector>

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
    nlohmann::json getJson(void);
#if 0
    cJSON* cJSONgetJson(void);
    const boost::json::value& getJsonBoostlib(void);
#endif
    int getTiledFirstgid(void) const;

    static const std::array<std::string, SPRITESHEET_POOL_SIZE> spritesheet_names;

private:
    std::string name;
    std::shared_ptr<SDL_Texture> texture;
    nlohmann::json json;
#if 0
    cJSON* json;
    boost::json::value jsonValueBoostlib;
#endif
    int tiledFirstgid;

    void loadTexture(std::string pathImage);
    void loadJson(std::string pathJson);
    void fetchFirstgid(Map& map);
    void set_tiled_firstgid(int new_tiled_firstgid);
};

#endif
