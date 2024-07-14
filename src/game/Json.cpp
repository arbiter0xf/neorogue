#if 0
// Remove when boostlib methods removed
#include <iostream>
#include <fstream>
#endif

#include <string.h>
#include <sys/stat.h>

#include "Json.hpp"
#include "Log.hpp"

cJSON* Json::readFromFile(std::string file_path)
{
    const int bytes = 1;
    const char* cjson_error_ptr;

    int ret;
    char* json_string;
    std::string msg;
    FILE* file;
    cJSON* json;
    struct stat file_stat;

    ret = 0;
    json = 0;
    file_stat = {};
    json_string = 0;

    ret = stat(file_path.c_str(), &file_stat);
    if (0 != ret) {
        msg = "Failed to stat ";
        msg += file_path;
        msg += " : ";
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }

    json_string = (char*) calloc(file_stat.st_size, bytes);
    printf("%s size is: %d\n", file_path.c_str(), file_stat.st_size);

    file = fopen(file_path.c_str(), "r");
    if (0 == file) {
        msg = "Failed to open file ";
        msg += file_path;
        msg += " : ";
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }

    ret = fread(json_string, bytes, file_stat.st_size, file);
    if (ret != file_stat.st_size) {
        free(json_string);
        json_string = 0;

        msg = "Read unexpected amount (";
        msg += ret;
        msg += ") of bytes from file ";
        msg += file_path;
        throw std::runtime_error(msg);
    }

    ret = fclose(file);
    if (0 != ret) {
        free(json_string);
        json_string = 0;

        msg = "Failed to close file ";
        msg += file_path;
        msg += " : ";
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }
    file = 0;

    json = cJSON_Parse(json_string);
    if (nullptr == json) {
        free(json_string);
        json_string = 0;

        cjson_error_ptr = cJSON_GetErrorPtr();

        msg = "Failed to cJSON_Parse";
        if (nullptr == cjson_error_ptr) {
            throw std::runtime_error(msg);
        }

        msg += ": ";
        msg += cjson_error_ptr;
        throw std::runtime_error(msg);
    }

    free(json_string);
    json_string = 0;

    return json;
}

#if 0
void Json::readFromFileBoostlib(std::string filename, boost::json::value& jsonValue)
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

#endif
