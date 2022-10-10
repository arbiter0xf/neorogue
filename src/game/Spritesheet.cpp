#include <stdexcept>

#include "Assets.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

const std::array<std::string, SPRITESHEET_POOL_SIZE> Spritesheet::spritesheetNames = {
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

const boost::json::value& Spritesheet::getJson(void)
{
    return jsonValue;
}

int Spritesheet::getTiledFirstgid(void) const
{
    return tiledFirstgid;
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
    Json::readFromFile(pathJson, jsonValue);
}

void Spritesheet::fetchFirstgid(Map& map)
{
    boost::json::value tmj = map.getTmj();

    if (boost::json::kind::object != tmj.kind()) {
        std::string msg = "While fetching firstgid, top level map .tmj ";
        msg += "JSON value is not an object";
        throw std::runtime_error(msg);
    }

    boost::json::value tilesetsArray = tmj
        .get_object()
        .find("tilesets")
        ->value();

    if (boost::json::kind::array != tilesetsArray.kind()) {
        throw std::runtime_error(
                "While fetching firstgid, JSON tilesets value is not an array");
    }

    for (boost::json::value tileset : tilesetsArray.as_array()) {
        std::string tilesetSourceStr;
        boost::json::value tilesetSource = tileset
            .get_object()
            .find("source")
            ->value();
        tilesetSourceStr = tilesetSource.as_string().c_str();

        if (0 != tilesetSourceStr.compare(name + ".tsx")) {
            continue;
        }

        boost::json::value tilesetFirstgid = tileset
            .get_object()
            .find("firstgid")
            ->value();

        tiledFirstgid = tilesetFirstgid.as_int64();

        return;
    }

    throw std::runtime_error("Could not find firstgid for spritesheet: " + name);
}
