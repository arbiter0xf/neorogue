#include <fstream>

#include <string.h>

#include "Json.hpp"
#include "Log.hpp"

nlohmann::json Json::readFromFile(std::string file_path)
{
    std::ifstream f(file_path.c_str());
    nlohmann::json data = nlohmann::json::parse(f);

    return data;
}
