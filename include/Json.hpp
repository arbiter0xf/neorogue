#ifndef JSON_HPP_DEFINED
#define JSON_HPP_DEFINED

#include <boost/json.hpp>

class Json
{
public:
    static void readFromFile(std::string filename, boost::json::value& jsonValue);
    static void texturepackerGetFrameObject(
            std::string frameKey,
            const boost::json::value& jsonValue,
            boost::json::object& frameObjectOut);
    static boost::json::value getValueWithKey(
            const char* key,
            const boost::json::object& frameObject);
    static boost::json::object getFirstInnerObject(boost::json::value& jsonValue);

private:
};

#endif // JSON_HPP_DEFINED
