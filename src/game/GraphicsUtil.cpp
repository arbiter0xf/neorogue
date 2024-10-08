#include "GraphicsUtil.hpp"

#include <array>
#include <filesystem>
#include <fstream>

#include "Log.hpp"

void GraphicsUtil::generateTiles(spritesheet_pool& spritesheet_pool, tile_pool& tile_pool)
{
    int spritesheet_tile_height;
    int spritesheet_tile_width;
    int spritesheet_width;

    nlohmann::json tmj;
    nlohmann::json item;

    std::shared_ptr<SDL_Texture> texture_spritesheet;

    spritesheet_tile_height = -1;
    spritesheet_tile_width = -1;
    spritesheet_width = -1;

    Log::d("clearing tile_pool");
    tile_pool.clear();

    for (Spritesheet& spritesheet : spritesheet_pool) {
        tmj = spritesheet.getJson();
        texture_spritesheet = spritesheet.getTexture();

        if ( ! tmj.is_object()) {
            throw std::runtime_error("Top level tmj JSON value is not an object");
        }

        Log::d("getting tile height and width");
        spritesheet_tile_height = tmj["tileheight"];
        spritesheet_tile_width = tmj["tilewidth"];

        if (32 != spritesheet_tile_height || 32 != spritesheet_tile_width) {
            throw std::runtime_error("Spritesheet tile size is not 32x32");
        }

        auto layers_array = tmj["layers"];
        if (1 != layers_array.size()) {
            throw std::runtime_error("Spritesheet JSON does not contain 1 layer");
        }

        item = layers_array[0];
        auto data_array = item["data"];
        spritesheet_width = item["width"];

        for (auto data_item : data_array) {
            // ID for spritesheet and GID for whole map
            unsigned int tiled_id = data_item;
            unsigned int tiled_gid = spritesheet.getTiledFirstgid() - 1 + tiled_id;

            if (0 == tiled_id) {
                break;
            }

            if (0 != (tiled_gid & g_constants::TILED_FLIPPED_HORIZONTALLY_FLAG)) {
                throw std::runtime_error("Horizontally flipped tiles not supported");
            }

            if (0 != (tiled_gid & g_constants::TILED_FLIPPED_VERTICALLY_FLAG)) {
                throw std::runtime_error("Vertically flipped tiles not supported");
            }

            if (0 != (tiled_gid & g_constants::TILED_FLIPPED_DIAGONALLY_FLAG)) {
                throw std::runtime_error("Diagonally flipped tiles not supported");
            }

            if (0 != (tiled_gid & g_constants::TILED_ROTATED_HEXAGONAL_120_FLAG)) {
                throw std::runtime_error("Rotated tiles not supported");
            }

            const int x = (tiled_id - 1) % spritesheet_width;
            const int y = (tiled_id - 1) / spritesheet_width;

            tile_pool[tiled_gid] = Tile(
                        "namePlaceholder",
                        texture_spritesheet,
                        x * g_constants::TILE_WIDTH,
                        y * g_constants::TILE_HEIGHT,
                        g_constants::TILE_WIDTH,
                        g_constants::TILE_HEIGHT,
                        tiled_gid,
                        tiled_id);
        }
    }
}

void GraphicsUtil::loadSpritesheets(spritesheet_pool& spritesheet_pool, Map& map)
{
    nlohmann::json tmj;

    std::string tileset_source;
    std::vector<std::string> tileset_names;

    tmj = map.getTmj();
    if ( ! tmj.is_object()) {
        throw std::runtime_error("Top level map .tmj JSON value is not an object");
    }

    auto tilesets = tmj["tilesets"];

    for (auto tileset : tilesets) {
        tileset_source = tileset["source"];

        tileset_names.push_back(tileset_source);
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
