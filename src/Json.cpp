#include <iostream>
#include <fstream>

#include "Json.hpp"
#include "Log.hpp"

void Json::readFromFile(std::string filename, boost::json::value& jsonValue)
{
    std::ifstream jsonStream;
    char buffer[4096] = {0};
    std::size_t readAmount = 0;
    boost::json::stream_parser jsonStreamParser;
    boost::json::error_code ec;

    jsonStream.open(filename, std::ifstream::in);
    if (jsonStream.fail()) {
        throw std::runtime_error("Failed to open json file: " + filename);
    }

    do {
        jsonStream.read(buffer, sizeof(buffer));
        readAmount = jsonStream.gcount();
#if DEBUG_VERBOSE
        std::cout << DBG << "Successfully read " << readAmount << " characters" << "\n";
#endif
        jsonStreamParser.write(buffer, readAmount, ec);
        if (ec) {
            throw std::runtime_error("Stream parser write failed");
        }
    } while(!jsonStream.eof());

    jsonStreamParser.finish(ec);
    if (ec) {
        throw std::runtime_error("Stream parser finish failed");
    }

    jsonStream.clear();
    jsonStream.close();
    if (jsonStream.fail()) {
        throw std::runtime_error("Failed to close json file: " + filename);
    }

    jsonValue = jsonStreamParser.release();
}

boost::json::value Json::getValueWithKey(
        const char* key,
        const boost::json::object& frameObject)
{
    auto frameObjectIter = frameObject.find(key);

#if DEBUG_VERBOSE
    std::cout << DBG << "frameObjectIter->key() is: " << frameObjectIter->key() << "\n";
    std::cout << DBG << "frameObjectIter->value() is: " << frameObjectIter->value() << "\n";
#endif

    return frameObjectIter->value();
}

// {"frames": {
//
//     "altars/dngn_altar.png":
//     {
//         "frame": {"x":35,"y":37,"w":32,"h":31},
//         "rotated": true,
//         "trimmed": true,
//         "spriteSourceSize": {"x":0,"y":1,"w":32,"h":31},
//         "sourceSize": {"w":32,"h":32}
//     },
//     "altars/dngn_altar_beogh.png":
//     {
/*
 * \exception Throws std::runtime_error on failure
 */
void Json::texturepackerGetFrameObject(
        std::string frameKey,
        const boost::json::value& jsonValue,
        boost::json::object& frameObjectOut)
{
    if (boost::json::kind::object != jsonValue.kind()) {
        throw std::runtime_error("JSON value top level is not an object");
    }

    auto const& topObj = jsonValue.get_object();
    auto iter = topObj.begin();
    boost::json::value framesValue = iter->value();

    auto frameIter = framesValue.get_object().find(frameKey);
#if DEBUG
    std::cout << DBG << "frameIter->key() is: " << frameIter->key() << "\n";
    std::cout << DBG << "frameIter->value() is: " << frameIter->value() << "\n";
#endif

    boost::json::value frameValue = frameIter->value();

    if (boost::json::kind::object != frameValue.kind()) {
        throw std::runtime_error("Frame JSON value is not an object");
    }

    frameObjectOut = frameValue.get_object();
}

boost::json::object Json::getFirstInnerObject(const boost::json::value& jsonValue)
{
    if (boost::json::kind::object != jsonValue.kind()) {
        throw std::runtime_error("Top level JSON value is not an object");
    }

    auto const& outerObj = jsonValue.get_object();
    if (outerObj.empty()) {
        throw std::runtime_error("Empty outer JSON object");
    }

    auto iter = outerObj.begin();
    return iter->value().get_object();
}
