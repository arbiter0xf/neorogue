#include "Log.hpp"
#include "Map.hpp"

Map::Map(std::string path)
{
    loadJson(path);
}

void Map::loadJson(std::string path)
{
    Json::readFromFile(path, mapTmj);
}

boost::json::value Map::getTmj(void)
{
    return mapTmj;
}

boost::json::value Map::getLayers(void)
{
    return mapTmj.get_object().find("layers")->value();
}

int Map::getLayerAmount(void)
{
    return getLayers().as_array().size();
}

std::uint32_t Map::getTiledGid(const int x, const int y, const int layerNum)
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
