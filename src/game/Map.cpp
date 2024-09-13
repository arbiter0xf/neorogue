#include "Log.hpp"
#include "Map.hpp"

Map::Map(std::string path)
{
    loadJson(path);
}

void Map::loadJson(std::string path)
{
    mapTmj = Json::readFromFile(path);
}

#if 0
void Map::cJSONloadJson(std::string path)
{
    mapTmj = Json::readFromFile(path);
}
#endif

nlohmann::json Map::getTmj(void)
{
    return mapTmj;
}

#if 0
boost::json::value Map::getTmjBoostlib(void)
{
    return mapTmj;
}
#endif

std::vector<nlohmann::json> Map::getLayers(void)
{
    return mapTmj["layers"];
}

#if 0
boost::json::value Map::getLayers(void)
{
    return mapTmj.get_object().find("layers")->value();
}
#endif

int Map::getLayerAmount(void)
{
    return getLayers().size();
}

#if 0
int Map::getLayerAmountBoostlib(void)
{
    return getLayers().as_array().size();
}
#endif

std::uint32_t Map::get_tiled_gid(const int x, const int y, const int layer_num)
{
    std::uint32_t tiled_gid;
    int layer_width;
    int layer_height;
    bool out_of_bounds;

    tiled_gid = 0;
    layer_width = 0;
    layer_height = 0;
    out_of_bounds = true;

    out_of_bounds = layer_num < 0 || layer_num >= getLayerAmount();
    if (out_of_bounds) {
        Log::w("Layer number out of bounds while getting tiled gid");
        return 0;
    }

    std::vector<nlohmann::json> layers_array = getLayers();
    nlohmann::json layer = layers_array[layer_num];
    std::vector<uint64_t> layer_data_array = layer["data"];

    layer_width = layer["width"];
    layer_height = layer["height"];

    out_of_bounds =
        x < 0 || x > (layer_width - 1) ||
        y < 0 || y > (layer_height - 1);
    if (out_of_bounds) {
        return 0;
    }

    tiled_gid = layer_data_array[(layer_width * y) + x];

    return tiled_gid;
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
