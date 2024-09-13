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

nlohmann::json Map::getTmj(void)
{
    return mapTmj;
}

std::vector<nlohmann::json> Map::getLayers(void)
{
    return mapTmj["layers"];
}

int Map::getLayerAmount(void)
{
    return getLayers().size();
}

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
