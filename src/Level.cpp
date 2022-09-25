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
    const int lineSize = 256;
    const char delimiter = ' ';
    std::ifstream fileStream;
    std::string lineStr;
    std::string extracted;
    char line[lineSize] = {0};
    std::vector<std::string> tileDescriptionRow;

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
                tileDescriptionRow.push_back(extracted);
            }
        } while(!lineStream.eof());

        tileDescriptions.push_back(tileDescriptionRow);

        tileDescriptionRow.clear();
    } while(!fileStream.eof());

    fileStream.clear();
    fileStream.close();
    if (fileStream.fail()) {
        throw std::runtime_error("Failed to close level file: " + path);
    }
}

std::string Level::getTileDescription(const int x, const int y)
{
    return tileDescriptions[y][x];
}
