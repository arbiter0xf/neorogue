#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "Level.hpp"
#include "Log.hpp"

Level::Level(std::string path)
{
    loadFromFile(path);
}

void Level::loadFromFile(std::string path)
{
    int tileId = 0;
    const int lineSize = 256;
    const char delimiter = ' ';
    std::ifstream fileStream;
    std::string lineStr;
    std::string extracted;
    char line[lineSize] = {0};
    std::vector<int> tileIdRow;

    fileStream.open(path, std::ifstream::in);
    if (fileStream.fail()) {
        throw std::runtime_error("Failed to open level file: " + path);
    }

    do {
        fileStream.getline(line, lineSize);
        lineStr = line;

        std::stringstream lineStream(lineStr);

        do {
            std::getline(lineStream, extracted, delimiter);
            // Multiple spaces produce empty strings, so check them before push.
            if ("" != extracted) {
                tileId = std::stoi(extracted);
                tileIdRow.push_back(tileId);
            }
        } while(!lineStream.eof());

        tileIds.push_back(tileIdRow);

        tileIdRow.clear();
    } while(!fileStream.eof());

    fileStream.clear();
    fileStream.close();
    if (fileStream.fail()) {
        throw std::runtime_error("Failed to close level file: " + path);
    }
}

int Level::getTileId(const unsigned int x, const unsigned int y)
{
    if (y >= tileIds.size()) {
        throw std::runtime_error("y value out of bounds when trying to access tileIds");
    }

    if (x >= tileIds[y].size()) {
        throw std::runtime_error("x value out of bounds when trying to access tileIds");
    }

    return tileIds[y][x];
}
