#include <stdexcept>

#include "Assets.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

const std::array<std::string, SPRITESHEET_POOL_SIZE> Spritesheet::spritesheetNames = {
    "dc-dngn",
    "dc-misc",
    "dc-mon",
    "effect",
    "gui",
    "item",
    "player",
    "spells",
};

#if GRAPHICS_ENABLED
Spritesheet::Spritesheet(std::string _name)
	: name {_name}
{
    loadTexture(Assets::spritesheetsDir + "/" + name + ".png");
    loadJson(Assets::spritesheetsDir + "/" + name + ".json");
}
#else
Spritesheet::Spritesheet(std::string _name)
	: name {_name}
{
    loadJson(Assets::spritesheetsDir + "/" + name + ".json");
}
#endif

std::string Spritesheet::getName(void)
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

boost::json::object Spritesheet::getTPackerFramesObject(void)
{
    return Json::getFirstInnerObject(getJson());
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
