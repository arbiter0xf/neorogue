#ifndef JSON_HPP_DEFINED
#define JSON_HPP_DEFINED

// Header-only with multiple translation units
// https://www.boost.org/doc/libs/1_69_0/libs/test/doc/html/boost_test/adv_scenarios/single_header_customizations/multiple_translation_units.html
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

private:
};

#endif // JSON_HPP_DEFINED
