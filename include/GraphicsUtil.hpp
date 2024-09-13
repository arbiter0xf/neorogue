#ifndef GRAPHICS_UTIL_HPP_DEFINED
#define GRAPHICS_UTIL_HPP_DEFINED

#include "Map.hpp"
#include "Tile.hpp"

class GraphicsUtil
{
public:
    static void generateTiles(spritesheet_pool& spritesheetPool, tile_pool& tilePool);
    static void loadSpritesheets(spritesheet_pool& spritesheetPool, Map& map);
    static void generateTileIdMapFile(tile_id_map& tileIdMap);

private:
};

#endif // GRAPHICS_UTIL_HPP_DEFINED
