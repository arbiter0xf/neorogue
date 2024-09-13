#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include "Json.hpp"

class Map
{
public:
    Map() = delete;
    Map(std::string path);

    nlohmann::json getTmj(void);
    std::vector<nlohmann::json> getLayers(void);
    int getLayerAmount(void);
    std::uint32_t get_tiled_gid(const int x, const int y, const int layerNum);

private:
    void loadJson(std::string path);

    nlohmann::json mapTmj;
};

#endif // MAP_HPP_DEFINED
