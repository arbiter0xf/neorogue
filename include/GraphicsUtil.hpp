#ifndef GRAPHICS_UTIL_HPP_DEFINED
#define GRAPHICS_UTIL_HPP_DEFINED

#include "Map.hpp"
#include "Tile.hpp"

class GraphicsUtil
{
public:
    static void generateTiles(spritesheet_pool& spritesheetPool, tile_pool& tilePool);
#if 0
    static void generateTilesBoostlib(spritesheet_pool& spritesheetPool, tile_pool& tilePool);
#endif

    static void loadSpritesheets(spritesheet_pool& spritesheetPool, Map& map);
#if 0
    static void loadSpritesheetsBoostlib(spritesheet_pool& spritesheetPool, Map& map);
#endif

    static void generateTileIdMapFile(tile_id_map& tileIdMap);

private:
};

#endif // GRAPHICS_UTIL_HPP_DEFINED
