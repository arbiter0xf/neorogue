#include "GraphicsUtil.hpp"

#include <array>
#include <filesystem>
#include <fstream>

#include "Log.hpp"

void GraphicsUtil::generateTiles(spritesheet_pool& spritesheetPool, tile_pool& tilePool)
{
    Log::w("GraphicsUtil::generateTiles not yet implemented");
}

#if 0
/*
 * \exception May throw std::exception
 */
void GraphicsUtil::generateTilesBoostlib(spritesheet_pool& spritesheetPool, tile_pool& tilePool)
{
    int spritesheetTileHeight = -1;
    int spritesheetTileWidth = -1;
    int spritesheetWidth = -1;

    tilePool.clear();

    for (Spritesheet& spritesheet : spritesheetPool) {
        boost::json::value tmj = spritesheet.getJson();
        std::shared_ptr<SDL_Texture> textureSpritesheet = spritesheet.getTexture();

        if (boost::json::kind::object != tmj.kind()) {
            throw std::runtime_error("Top level Tiled JSON value is not an object");
        }

        spritesheetTileHeight = tmj
            .get_object()
            .find("tileheight")
            ->value()
            .as_int64();

        spritesheetTileWidth = tmj
            .get_object()
            .find("tilewidth")
            ->value()
            .as_int64();

        if (32 != spritesheetTileHeight || 32 != spritesheetTileWidth) {
            throw std::runtime_error("Spritesheet tile size is not 32x32");
        }

        boost::json::value layersArray = tmj
            .get_object()
            .find("layers")
            ->value();

        if (layersArray.as_array().size() > 1) {
            throw std::runtime_error("More than 1 layer in a spritesheet JSON");
        }

        boost::json::value layerData = layersArray.as_array()[0]
            .get_object()
            .find("data")
            ->value();

        spritesheetWidth = layersArray.as_array()[0]
            .get_object()
            .find("width")
            ->value()
            .as_int64();

        if (boost::json::kind::array != layerData.kind()) {
            throw std::runtime_error("Unsupported spritesheet layer data");
        }

        for (boost::json::value dataItem : layerData.as_array()) {
            // ID for spritesheet and GID for whole map
            unsigned tiledId = dataItem.as_int64();
            unsigned tiledGid = spritesheet.getTiledFirstgid() - 1 + tiledId;

            if (0 == tiledId) {
                break;
            }

            if (0 != (tiledGid & g_constants::TILED_FLIPPED_HORIZONTALLY_FLAG)) {
                throw std::runtime_error("Horizontally flipped tiles not supported");
            }

            if (0 != (tiledGid & g_constants::TILED_FLIPPED_VERTICALLY_FLAG)) {
                throw std::runtime_error("Vertically flipped tiles not supported");
            }

            if (0 != (tiledGid & g_constants::TILED_FLIPPED_DIAGONALLY_FLAG)) {
                throw std::runtime_error("Diagonally flipped tiles not supported");
            }

            if (0 != (tiledGid & g_constants::TILED_ROTATED_HEXAGONAL_120_FLAG)) {
                throw std::runtime_error("Rotated tiles not supported");
            }

            const int x = (tiledId - 1) % spritesheetWidth;
            const int y = (tiledId - 1) / spritesheetWidth;

            tilePool[tiledGid] = Tile(
                        "namePlaceholder",
                        textureSpritesheet,
                        x * g_constants::TILE_WIDTH,
                        y * g_constants::TILE_HEIGHT,
                        g_constants::TILE_WIDTH,
                        g_constants::TILE_HEIGHT,
                        tiledGid,
                        tiledId);
        }
    }
}
#endif

void GraphicsUtil::loadSpritesheets(spritesheet_pool& spritesheet_pool, Map& map)
{
    cJSON* tmj;
    cJSON* item;
    cJSON* tileset;
    cJSON* tileset_source;

    std::vector<std::string> tileset_names;

    tmj = 0;
    item = 0;
    tileset = 0;
    tileset_source = 0;

    tmj = map.getTmj();
    if ( ! cJSON_IsObject(tmj)) {
        throw std::runtime_error("Top level map .tmj JSON value is not an object");
    }

    item = cJSON_GetObjectItemCaseSensitive(tmj, "tilesets");
    if ( ! cJSON_IsArray(item)) {
        throw std::runtime_error("JSON tilesets value is not an array");
    }

    cJSON_ArrayForEach(tileset, item) {
        tileset_source = cJSON_GetObjectItemCaseSensitive(tileset, "source");
        if ( ! cJSON_IsString(tileset_source)) {
            throw std::runtime_error("Tileset source is not a string");
        }

        tileset_names.push_back(tileset_source->valuestring);
    }

    for (std::string spritesheet_name : Spritesheet::spritesheet_names) {
        for (std::string tileset_name : tileset_names) {
            if (0 == tileset_name.compare(spritesheet_name + ".tsx")) {
                spritesheet_pool.push_back(Spritesheet(spritesheet_name, map));
                break;
            }
        }
    }
}

// Keeping as a reference for using std::filesystem
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
