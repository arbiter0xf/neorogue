#include "GraphicsUtil.hpp"

#include <filesystem>
#include <fstream>

/*
 * \exception May throw std::exception
 */
void GraphicsUtil::generateTiles(spritesheet_pool& spritesheetPool, tile_pool& tilePool)
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

void GraphicsUtil::loadSpritesheets(spritesheet_pool& spritesheetPool)
{
    std::transform(
            Spritesheet::spritesheetNames.cbegin(),
            Spritesheet::spritesheetNames.cend(),
            spritesheetPool.begin(),
            [](const std::string name) {
                return Spritesheet(name);
            });
}

void GraphicsUtil::generateTileIdMap(
        spritesheet_pool& spritesheetPool,
        tile_id_map& tileIdMap)
{
    int currentId = 0;

    for (Spritesheet spritesheet : spritesheetPool) {
        boost::json::object framesObject = spritesheet.getTPackerFramesObject();
        auto framesIter = framesObject.cbegin();
        auto framesIterEnd = framesObject.cend();

        while (framesIter != framesIterEnd) {
            const auto& [tileName, tileConfig] = *framesIter;

            tileIdMap[currentId] = tileName;

            currentId++;
            framesIter++;
        }
    }
}

void GraphicsUtil::generateTileIdMapFile(tile_id_map& tileIdMap)
{
    int currentId = 0;
    const std::string tileIdMapFileName = "generated/tile_id_map.txt";
    std::ofstream outStream;
    std::string fileContent;

    const std::filesystem::path dirGenerated = {"generated"};

    if (!std::filesystem::exists(dirGenerated)) {
        std::filesystem::create_directory(dirGenerated);
    }

    outStream.open(tileIdMapFileName, std::ios::out);
    if (outStream.fail()) {
        throw std::runtime_error("Failed to open file: " + tileIdMapFileName + " for writing");
    }

    for (std::string tileName : tileIdMap) {
        fileContent += std::to_string(currentId);
        fileContent += ": ";
        fileContent += tileName;
        fileContent += "\n";
        currentId++;
    }

    outStream.write(fileContent.c_str(), fileContent.size());
}
