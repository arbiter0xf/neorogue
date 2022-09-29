#ifndef CONSTANTS_HPP_DEFINED
#define CONSTANTS_HPP_DEFINED

#include <string>
// #include <string_view>

// https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/

namespace g_constants {
    inline constexpr int TILE_HEIGHT = 32;
    inline constexpr int TILE_WIDTH = 32;
    inline constexpr int SCREEN_WIDTH = 640;
    inline constexpr int SCREEN_HEIGHT = 480;
    inline constexpr int SCREEN_TILES = (SCREEN_WIDTH / TILE_WIDTH) * (SCREEN_HEIGHT / TILE_HEIGHT) + 1; // +1 for null termination
    inline constexpr int TILE_POOL_SIZE = 3137; // TODO write a script for getting this and
                                                // pass by using -DTILE_POOL_SIZE=$(script)
    inline constexpr int TEXTURE_POOL_SIZE = 98; // TODO write a script for getting this and
                                                 // pass by using -DTEXUTRE_POOL_SIZE=$(script)
    // inline constexpr std::string PROGRAM_NAME = "Rogue Forever";
    inline const char PROGRAM_NAME[] = "Rogue Forever";
}

#endif // CONSTANTS_HPP_DEFINED
