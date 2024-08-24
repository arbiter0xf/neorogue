#include "Log.hpp"
#include "Map.hpp"

Map::Map(std::string path)
{
    loadJson(path);
}

void Map::loadJson(std::string path)
{
    Log::w("Map::loadJson not yet implemented");
}

#if 0
void Map::cJSONloadJson(std::string path)
{
    mapTmj = Json::readFromFile(path);
}
#endif

cJSON* Map::getTmj(void)
{
    return mapTmj;
}

#if 0
boost::json::value Map::getTmjBoostlib(void)
{
    return mapTmj;
}
#endif

cJSON* Map::getLayers(void)
{
    Log::w("Map::getLayers not yet implemented");
    return nullptr;
}

#if 0
boost::json::value Map::getLayers(void)
{
    return mapTmj.get_object().find("layers")->value();
}
#endif

int Map::getLayerAmount(void)
{
    Log::w("Map::getLayerAmount not yet implemented");
    return -1;
}

#if 0
int Map::getLayerAmountBoostlib(void)
{
    return getLayers().as_array().size();
}
#endif

std::uint32_t Map::getTiledGid(const int x, const int y, const int layerNum)
{
    Log::w("Map::getTiledGid not yet implemented");
}

#if 0
std::uint32_t Map::getTiledGidBoostlib(const int x, const int y, const int layerNum)
{
    std::uint32_t tiledGid = 0;
    int layerWidth = 0;
    int layerHeight = 0;
    bool outOfBounds = true;

    outOfBounds = layerNum < 0 || layerNum >= getLayerAmount();
    if (outOfBounds) {
        return 0;
    }

    boost::json::value layersArray = getLayers();
    boost::json::value layer = layersArray.as_array()[layerNum];
    boost::json::array layerDataArray = layer
        .as_object()
        .find("data")
        ->value()
        .as_array();

    layerWidth = layer.as_object().find("width")->value().as_int64();
    layerHeight = layer.as_object().find("height")->value().as_int64();

    outOfBounds =
        x < 0 || x > (layerWidth - 1) ||
        y < 0 || y > (layerHeight - 1);
    if (outOfBounds) {
        return 0;
    }

    tiledGid = layerDataArray[(layerWidth * y) + x].as_int64();

    return tiledGid;
}
#endif
