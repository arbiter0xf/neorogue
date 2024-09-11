#include <array>
#include <stdexcept>

#include "Assets.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

const std::array<std::string, SPRITESHEET_POOL_SIZE> Spritesheet::spritesheet_names = {
    "dc-dngn-collection-spritesheet",
    "dc-misc-collection-spritesheet",
    "dc-mon-collection-spritesheet",
    "effect-collection-spritesheet",
    "gui-collection-spritesheet",
    "item-collection-spritesheet",
    "player-collection-spritesheet",
    "spells-collection-spritesheet",
};

#if GRAPHICS_ENABLED
Spritesheet::Spritesheet(std::string _name, Map& map)
	: name {_name}
{
    loadTexture(Assets::spritesheetsDir + "/" + name + ".png");
    loadJson(Assets::spritesheetsDir + "/" + name + ".tmj");
    fetchFirstgid(map);
}
#elif BUILD_FOR_TESTS
Spritesheet::Spritesheet(std::string _name, Map& map)
	: name {_name}
{
    loadJson(Assets::testDataDir + "/" + name + ".tmj");
    fetchFirstgid(map);
}
#else
#error "Unknown build type"
#endif

std::string Spritesheet::getName(void) const
{
    return name;
}

std::shared_ptr<SDL_Texture> Spritesheet::getTexture(void)
{
    return texture;
}

nlohmann::json Spritesheet::getJson(void)
{
    return json;
}

#if 0
cJSON* Spritesheet::cJSONgetJson(void)
{
    return json;
}
#endif

int Spritesheet::getTiledFirstgid(void) const
{
    return tiledFirstgid;
}

void Spritesheet::set_tiled_firstgid(int new_tiled_firstgid)
{
    tiledFirstgid = new_tiled_firstgid;
}

void Spritesheet::loadTexture(std::string pathImage)
{
    Sdlw& sdlw = Sdlw::getReference();

    texture = sdlw.imgLoadTextureShared(pathImage);
    if (NULL == texture) {
        throw std::runtime_error("Failed to load texture: " + pathImage);
    }
}

void Spritesheet::loadJson(std::string pathJson)
{
    json = Json::readFromFile(pathJson);
}

#if 0
void Spritesheet::cJSONloadJson(std::string pathJson)
{
    json = Json::readFromFile(pathJson);
}
#endif

void Spritesheet::fetchFirstgid(Map& map)
{
    int tilesets_array_size;

    std::string tileset_source_str;

    nlohmann::json tmj;
    nlohmann::json tileset;

    const std::string spritesheet_name = getName();

    tilesets_array_size = -1;
    tileset_source_str = "N/A";

    tmj = map.getTmj();

    if ( ! tmj.is_object()) {
        std::string msg = "While fetching firstgid, top level map .tmj ";
        msg += "JSON value is not an object";
        throw std::runtime_error(msg);
    }

    auto tilesets_array = tmj["tilesets"];
    tilesets_array_size = tmj["tilesets"].size();

    for (int i = 0; i < tilesets_array_size; ++i) {
        tileset = tilesets_array[i];
        tileset_source_str = tileset["source"];

        if (0 != tileset_source_str.compare(spritesheet_name + ".tsx")) {
            continue;
        }

        auto tileset_firstgid = tileset["firstgid"];

        set_tiled_firstgid(tileset_firstgid);

        return;
    }

    throw std::runtime_error("Could not find firstgid for spritesheet: " + name);
}

#if 0
void Spritesheet::cJSONfetchFirstgid(Map& map)
{
    int tilesets_array_size;

    std::string tileset_source_str;

    cJSON* tmj;
    cJSON* item;
    cJSON* tileset;
    cJSON* tileset_source;
    cJSON* tileset_firstgid;

    const std::string spritesheet_name = getName();

    tmj = 0;
    item = 0;
    tileset = 0;
    tileset_source = 0;
    tileset_firstgid = 0;

    tilesets_array_size = -1;

    tileset_source_str = "N/A";

    tmj = map.getTmj();

    if ( ! cJSON_IsObject(tmj)) {
        std::string msg = "While fetching firstgid, top level map .tmj ";
        msg += "JSON value is not an object";
        throw std::runtime_error(msg);
    }

    item = cJSON_GetObjectItemCaseSensitive(tmj, "tilesets");
    if ( ! cJSON_IsArray(item)) {
        throw std::runtime_error(
            "While fetching firstgid, JSON tilesets value is not an array");
    }

    tilesets_array_size = cJSON_GetArraySize(item);
    for (int i = 0; i < tilesets_array_size; ++i) {
        tileset = cJSON_GetArrayItem(item, i);
        tileset_source = cJSON_GetObjectItemCaseSensitive(tileset, "source");
        tileset_source_str = tileset_source->valuestring;

        if (0 != tileset_source_str.compare(spritesheet_name + ".tsx")) {
            continue;
        }

        tileset_firstgid = cJSON_GetObjectItemCaseSensitive(
            tileset, "firstgid");

        set_tiled_firstgid(tileset_firstgid->valueint);

        return;
    }

    throw std::runtime_error("Could not find firstgid for spritesheet: " + name);
}
#endif
