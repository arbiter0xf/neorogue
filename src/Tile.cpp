#include "Sdlw.hpp"
#include "Tile.hpp"

Tile::~Tile()
{
}

Tile::Tile(
        const int _screenX,
        const int _screenY,
        std::string _name,
        std::shared_ptr<SDL_Texture> _texture,
        const int _sheetX,
        const int _sheetY,
        const int _sheetW,
        const int _sheetH)
        :
            screenX{_screenX},
            screenY{_screenY},
            name{_name},
            sheetTexture{_texture},
            sheetX{_sheetX},
            sheetY{_sheetY},
            sheetW{_sheetW},
            sheetH{_sheetH}
{
}

/*
 * \exception May throw std::exception
 */
void Tile::generateTiles(spritesheet_pool& spritesheetPool, tile_pool& tilePool)
{
    int ret = -1;

    boost::json::object tPackerFramesObj;

    for (Spritesheet& spritesheet : spritesheetPool) {
        boost::json::value tPackerJsonValue = spritesheet.getJson();
        std::shared_ptr<SDL_Texture> textureSpritesheet = spritesheet.getTexture();

        tPackerFramesObj = Json::getFirstInnerObject(tPackerJsonValue);

	for (const auto& [tileName, tileConfig] : tPackerFramesObj) {
            const auto tileConfigObj = tileConfig.as_object();
            const auto tileConfigFrameObj =
                Json::getValueWithKey("frame", tileConfigObj).as_object();

            tilePool[tileName] = Tile(
                    -1,
                    -1,
                    tileName,
                    textureSpritesheet,
                    Json::getValueWithKey("x", tileConfigFrameObj).as_int64(),
                    Json::getValueWithKey("y", tileConfigFrameObj).as_int64(),
                    Json::getValueWithKey("w", tileConfigFrameObj).as_int64(),
                    Json::getValueWithKey("h", tileConfigFrameObj).as_int64());
	}

#if 0
        // TODO See also:
        // https://en.cppreference.com/w/cpp/algorithm/generate
        std::transform(
                tPackerFramesObj.cbegin(),
                tPackerFramesObj.cend(),
                tilePoolIter,
                [&](const auto frameObj) {
                    const auto& [tileName, tileConfig] = frameObj;
                    const auto tileConfigObj = tileConfig.as_object();
                    const auto tileConfigFrameObj = Json::getValueWithKey("frame", tileConfigObj).as_object();

                    return Tile(
                            -1,
                            -1,
                            tileName,
                            textureSpritesheet,
                            Json::getValueWithKey("x", tileConfigFrameObj).as_int64(),
                            Json::getValueWithKey("y", tileConfigFrameObj).as_int64(),
                            Json::getValueWithKey("w", tileConfigFrameObj).as_int64(),
                            Json::getValueWithKey("h", tileConfigFrameObj).as_int64());
                });

        // Iterator seemed to point to where it pointed before calling
        // std::transform(). Walk to first uninitialized Tile in tilePool.
        while (NULL != tilePoolIter->getSheetTexture()) {
            if (tilePool.end() == tilePoolIter) {
                throw std::runtime_error("Reached end of tile pool when generating tiles");
            }

            tilePoolIter++;
        }
#endif
    }
}

const int Tile::getScreenX() const
{
    return screenX;
}

const int Tile::getScreenY() const
{
    return screenY;
}

const std::string Tile::getName() const
{
    return name;
}

std::shared_ptr<SDL_Texture> Tile::getSheetTexture()
{
    return sheetTexture;
}

const int Tile::getSheetX() const
{
    return sheetX;
}

const int Tile::getSheetY() const
{
    return sheetY;
}

const int Tile::getSheetW() const
{
    return sheetW;
}

const int Tile::getSheetH() const
{
    return sheetH;
}
