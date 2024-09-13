#ifndef JSON_HPP_DEFINED
#define JSON_HPP_DEFINED

#include <nlohmann/json.hpp>

#include <string>

class Json
{
public:
    static nlohmann::json readFromFile(std::string filename);

private:
};

#endif // JSON_HPP_DEFINED
