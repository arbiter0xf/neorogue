#ifndef CONSTANTS_HPP_DEFINED
#define CONSTANTS_HPP_DEFINED

#include <string>

#define DEBUG 1
#define DEBUG_VERBOSE 0

// https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/

namespace g_constants {
    inline constexpr int TILE_HEIGHT = 32;
    inline constexpr int TILE_WIDTH = 32;
    inline constexpr int SCREEN_WIDTH = 800;
    inline constexpr int SCREEN_HEIGHT = 600;
    inline constexpr int TILES_HORIZONTAL = (SCREEN_WIDTH / TILE_WIDTH);
    inline constexpr int TILES_VERTICAL = (SCREEN_HEIGHT / TILE_HEIGHT);
    inline constexpr int SCREEN_TILES = TILES_HORIZONTAL * TILES_VERTICAL + 1; // +1 for null termination
    inline constexpr int TILE_AMOUNT = 3137; // TODO write a script for getting this and
                                             // pass by using -DTILE_POOL_SIZE=$(script)
    inline constexpr int TEXTURE_POOL_SIZE = 98; // TODO write a script for getting this and
                                                 // pass by using -DTEXUTRE_POOL_SIZE=$(script)
    inline constexpr char PROGRAM_NAME[] = "Rogue Forever";
    inline constexpr std::uint32_t TILED_FLIPPED_HORIZONTALLY_FLAG  = 0x80000000;
    inline constexpr std::uint32_t TILED_FLIPPED_VERTICALLY_FLAG    = 0x40000000;
    inline constexpr std::uint32_t TILED_FLIPPED_DIAGONALLY_FLAG    = 0x20000000;
    inline constexpr std::uint32_t TILED_ROTATED_HEXAGONAL_120_FLAG = 0x10000000;
    inline constexpr bool DEBUG_FRAME_STEPPING = false;
}

#endif // CONSTANTS_HPP_DEFINED
