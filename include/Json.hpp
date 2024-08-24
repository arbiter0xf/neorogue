#ifndef JSON_HPP_DEFINED
#define JSON_HPP_DEFINED

#if 0
#include <boost/json.hpp>
#endif

#include <cjson/cJSON.h>
#include <nlohmann/json.hpp>

#include <string>

class Json
{
public:
    static nlohmann::json readFromFile(std::string filename);
    // static cJSON* readFromFile(std::string filename);
#if 0
    static void readFromFileBoostlib(std::string filename, boost::json::value& jsonValue);
    static void texturepackerGetFrameObject(
            std::string frameKey,
            const boost::json::value& jsonValue,
            boost::json::object& frameObjectOut);
    static boost::json::value getValueWithKey(
            const char* key,
            const boost::json::object& frameObject);
    static boost::json::object getFirstInnerObject(const boost::json::value& jsonValue);
#endif

private:
};

#endif // JSON_HPP_DEFINED
