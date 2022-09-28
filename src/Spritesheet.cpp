#include <stdexcept>

#include "Assets.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

Spritesheet::~Spritesheet()
{
    Sdlw& sdlw = Sdlw::getReference();

    sdlw.destroyTexture(texture);
}

Spritesheet::Spritesheet(std::string _name)
	: name {_name}
{
    loadTexture(Assets::spritesheetsDir + "/" + name + ".png");
    loadJson(Assets::spritesheetsDir + "/" + name + ".json");
}

std::string Spritesheet::getName(void)
{
    return name;
}

SDL_Texture* Spritesheet::getTexture(void)
{
    return texture;
}

boost::json::value Spritesheet::getJson(void)
{
    return jsonValue;
}

void Spritesheet::loadTexture(std::string pathImage)
{
    Sdlw& sdlw = Sdlw::getReference();

    texture = sdlw.imgLoadTexture(pathImage);
    if (NULL == texture) {
        throw std::runtime_error("Failed to load texture: " + pathImage);
    }
}

void Spritesheet::loadJson(std::string pathJson)
{
    Json::readFromFile(pathJson, jsonValue);
}
