#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include "Json.hpp"

class Map
{
public:
    Map() = delete;
    Map(std::string path);

    nlohmann::json getTmj(void);
#if 0
    cJSON* getTmj(void);
    boost::json::value getTmjBoostlib(void);
#endif

    cJSON* getLayers(void);
#if 0
    boost::json::value getLayersBoostlib(void);
#endif

    int getLayerAmount(void);
#if 0
    int getLayerAmountBoostlib(void);
#endif
    std::uint32_t getTiledGid(const int x, const int y, const int layerNum);

private:
    void loadJson(std::string path);

    nlohmann::json mapTmj;
#if 0
    cJSON* mapTmj;
    boost::json::value mapTmjBoostlib;
#endif
};

#endif // MAP_HPP_DEFINED
