#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include "Json.hpp"

class Map
{
public:
    Map() = delete;
    Map(std::string path);

    boost::json::value getTmj(void);
    boost::json::value getLayers(void);
    int getLayerAmount(void);
    std::uint32_t getTiledGid(const int x, const int y, const int layerNum);

private:
    void loadJson(std::string path);

    boost::json::value mapTmj;
};

#endif // MAP_HPP_DEFINED
