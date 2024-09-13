#ifndef LEVEL_HPP_DEFINED
#define LEVEL_HPP_DEFINED

#include <string>

class Level
{
public:
    Level() = delete;
    Level(std::string path);

    int getTileId(const unsigned int x, const unsigned int y);

private:
    void loadFromFile(std::string path);

    std::vector<std::vector<int>> tileIds;
};

#endif // LEVEL_HPP_DEFINED
