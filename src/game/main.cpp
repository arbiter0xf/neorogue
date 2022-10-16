#include <boost/asio.hpp>

// Header-only with multiple translation units
// https://www.boost.org/doc/libs/1_69_0/libs/test/doc/html/boost_test/adv_scenarios/single_header_customizations/multiple_translation_units.html
// Must to include boost/json/src.hpp in only one source file and other files
// must then include boost/json.hpp.
#include <boost/json/src.hpp>

#include <stdio.h>

#include <array>
#include <iostream>
#include <string>

#include "Assets.hpp"
#include "Constants.hpp"
#include "GraphicsUtil.hpp"
#include "Json.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include "Map.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"
#include "Tile.hpp"

using screen_tiles = std::array<std::array<Tile*, g_constants::TILES_HORIZONTAL>, g_constants::TILES_VERTICAL>;
using screen_tiles_layers = std::vector<screen_tiles>;
using texture_pool = std::array<SDL_Texture*, g_constants::TEXTURE_POOL_SIZE>;

#if 0
void mainLoop(void) {
}
#endif

void renderTile(
        Sdlw& sdlw,
        Tile* tile,
        int screenX,
        int screenY)
{
    SDL_Rect srcRect = {
        tile->getSheetX(),
        tile->getSheetY(),
        tile->getSheetW(),
        tile->getSheetH(),
    };
    SDL_Rect dstRect = {
        screenX,
        screenY,
        tile->getSheetW(),
        tile->getSheetH()
    };

#if DEBUG_VERBOSE
    Log::d("Dumping tile which is getting rendered:");
    std::string msg = tile->getDump();
    Log::d(msg);
#endif

    sdlw.renderCopy(tile->getSheetTexture(), &srcRect, &dstRect);
}

void fillScreenTiles(
        tile_pool& tilePool,
        Map& map,
        tile_id_map& tileIdMap,
        int cameraX,
        int cameraY,
        screen_tiles_layers& screenTilesLayers)
{
    screen_tiles screenTiles;

    const int screenTilesColumnSize = screenTiles.size();
    const int screenTilesRowSize = screenTiles[0].size();

    // Camera is considered to be in the center of the screen. Shift half a
    // screen left and up and eventually draw level from there.
    int levelY = cameraY - (g_constants::TILES_VERTICAL / 2);
    int levelX = cameraX - (g_constants::TILES_HORIZONTAL / 2);
    int levelYEnd = levelY + screenTilesColumnSize;
    int levelXEnd = levelX + screenTilesRowSize;

    const int layerAmount = map.getLayerAmount();

    screenTilesLayers.clear();

    if (levelYEnd > screenTilesColumnSize) {
        levelYEnd = screenTilesColumnSize;
    }

    if (levelXEnd > screenTilesRowSize) {
        levelXEnd = screenTilesRowSize;
    }

    for (int layerNum = 0; layerNum < layerAmount; ++layerNum) {
        for (auto& tilesRow : screenTiles) {
            tilesRow.fill(0);
        }

        levelY = cameraY - (g_constants::TILES_VERTICAL / 2);

        for (int screenTileY = 0; screenTileY < screenTilesColumnSize; ++screenTileY) {
            if (levelY < 0) {
                ++levelY;
                continue;
            }

            if (levelY >= levelYEnd) {
                break;
            }

            levelX = cameraX - (g_constants::TILES_HORIZONTAL / 2);

            for (int screenTileX = 0; screenTileX < screenTilesRowSize; ++screenTileX) {
                if (levelX < 0) {
                    ++levelX;
                    continue;
                }

                if (levelX >= levelXEnd) {
                    break;
                }

                std::uint32_t tiledGid = map.getTiledGid(levelX, levelY, layerNum);
                // Leave as null if gid zero
                if (0 != tiledGid) {
                    screenTiles[screenTileY][screenTileX] = &(tilePool[tiledGid]);
                }

                ++levelX;
            }

            ++levelY;
        }

        screenTilesLayers.push_back(screenTiles);
    }
}

void renderScreenTiles(
        Sdlw& sdlw,
        const screen_tiles& screenTiles)
{
    for (int screenY = 0; screenY < screenTiles.size(); screenY++) {
        for (int screenX = 0; screenX < screenTiles[0].size(); screenX++) {
            if (NULL == screenTiles[screenY][screenX]) {
                continue;
            }

            renderTile(
                    sdlw,
                    screenTiles[screenY][screenX],
                    screenX * g_constants::TILE_WIDTH,
                    screenY * g_constants::TILE_HEIGHT);
        }
    }
}

void renderScreenTilesLayers(
        Sdlw& sdlw,
        const screen_tiles_layers& screenTilesLayers)
{
    for (const screen_tiles& screenTiles : screenTilesLayers) {
        renderScreenTiles(sdlw, screenTiles);
    }
}

#if DEBUG_VERBOSE
void printScreenTilesLayers(screen_tiles_layers& screenTilesLayers)
{
    Log::d("Printing screenTilesLayers:");
    for (auto& screenTiles : screenTilesLayers) {
        Log::d("Printing screenTiles:");
        for (auto& tilesRow : screenTiles) {
            for (Tile* tilePtr : tilesRow) {
                if (NULL == tilePtr) {
                    std::cout << 0;
                    std::cout << ", ";
                } else {
                    std::cout << tilePtr->getTiledGid();
                    std::cout << ", ";
                }
            }

            std::cout << "\n";
        }
    }
}
#endif

#if DEBUG_VERBOSE
void printTilesFromTilePool(tile_pool& tilePool)
{
    Log::d("Printing 10 first tiles:");
    for (int i = 0; i < 10; ++i) {
        std::string msg = "\n" + tilePool[i].getDump();
        Log::d(msg);
    }

    Log::d("Printing specific tiles:");
    std::string tempMsg = "\n" + tilePool[580].getDump();
    Log::d(tempMsg);
    tempMsg = "\n" + tilePool[700].getDump();
    Log::d(tempMsg);
}
#endif

#if DEBUG
void printConstructedSpritesheets(const spritesheet_pool& spritesheetPool)
{
    for (const Spritesheet& spritesheet : spritesheetPool) {
        std::string msg = "Constructed spritesheet: ";
        msg += spritesheet.getName();
        msg += " (";
        msg += std::to_string(spritesheet.getTiledFirstgid());
        msg += ")";
        Log::d(msg);
    }
}
#endif

void maybeDownloadContent()
{
    const int SERVER_CONTENT_PORT = 9002;
    const char* REQUEST_GET_MAPS = "get maps";
    const char* REQUEST_GET_MAP_ASSETS = "get map assets";
    const char END_OF_TRANSMISSION = (char) 4;
    boost::asio::io_context io_context;
    boost::system::error_code error;
    std::size_t bytesTransferred = 0;
    std::string errorMsg;

    std::string dynBuf;

    // TODO check file existence

    Log::i("Downloading content");

    try {
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve("localhost", std::to_string(SERVER_CONTENT_PORT));

        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        dynBuf = REQUEST_GET_MAPS;
        dynBuf.append(1, END_OF_TRANSMISSION);

        Log::i("Getting maps");
        bytesTransferred = write(socket, boost::asio::dynamic_buffer(dynBuf), error);
        if (bytesTransferred <= 0) {
            errorMsg = "Failed to write to socket: ";
            errorMsg += error.value();
            Log::e(errorMsg);
            return;
        }

        if (socket.is_open()) {
            socket.shutdown(tcp::socket::shutdown_type::shutdown_both);
        }
        socket.close();
    } catch(std::exception& e) {
        errorMsg = "Exception while downloading content: ";
        errorMsg += e.what();
        Log::e(errorMsg);
    }
}

void game(void)
{
    int err = -1;
    int ret = -1;

    int cameraY = g_constants::TILES_VERTICAL / 2;
    int cameraX = g_constants::TILES_HORIZONTAL / 2;

    bool quitEventReceived = false;

    Sdlw& sdlw = Sdlw::getReference();

    SDL_Event event;

    tile_pool tilePool;
    tile_id_map tileIdMap;
    texture_pool texturePool;
    spritesheet_pool spritesheetPool;

    screen_tiles_layers screenTilesLayers;

    maybeDownloadContent();

    Log::i("Loading map2");
    Map currentMap = Map("maps/map2_16x16_redone.tmj");

    Log::i("Initializing rendering");
    try {
        Sdlw::initRendering();
    } catch(std::exception& e) {
        std::cerr << "Exception while initializing rendering: " << e.what();
        throw e;
    }

    Log::i("Loading spritesheets");
    GraphicsUtil::loadSpritesheets(spritesheetPool, currentMap);

#if DEBUG
    printConstructedSpritesheets(spritesheetPool);
#endif

    Log::i("Generating tiles");
    try {
        GraphicsUtil::generateTiles(spritesheetPool, tilePool);
    } catch (std::exception const& e) {
        std::cerr << ERR << "Exception while generating tiles from spritesheets: "
            << e.what() << "\n";
        throw e;
    }

#if DEBUG_VERBOSE
    printTilesFromTilePool(tilePool);
#endif

    Log::i("Entering main loop");
    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        sdlw.renderClear();

        fillScreenTiles(tilePool,
			currentMap,
			tileIdMap,
			cameraX,
			cameraY,
			screenTilesLayers);

#if DEBUG_VERBOSE
        printScreenTilesLayers(screenTilesLayers);
#endif

        renderScreenTilesLayers(sdlw, screenTilesLayers);

        sdlw.renderPresent();

        if (g_constants::DEBUG_FRAME_STEPPING) {
            std::string readHere = "";
            std::getline(std::cin, readHere);
        }
    }

#if 0
    mainLoop();
#endif

    sdlw.destroy();
}

// Disable name mangling so that SDL can find and redefine main.
// https://djrollins.com/2016/10/02/sdl-on-windows/
extern "C" int main(int argc, char* argv[])
{
    try {
        game();
    } catch(std::exception const& e) {
        std::string msg = "Terminating due to unhandled exception: ";
        msg += e.what();
        Log::e(msg);
    }

    return 0;
}
