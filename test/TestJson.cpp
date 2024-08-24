#include "gtest/gtest.h"

#include "Json.hpp"

class TestJson : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestJson, ParseJsonFromFile)
{
    nlohmann::json json;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    json = nullptr;
    json = Json::readFromFile(file_path);

    EXPECT_NE(json, nullptr);
}

#if 0
TEST_F(TestJson, cJSONParseJsonFromFile)
{
    cJSON* json;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    json = 0;

    json = Json::readFromFile(file_path);

    EXPECT_NE(json, nullptr);
}
#endif

TEST_F(TestJson, ReadMapTileHeight)
{
    nlohmann::json json;
    uint64_t tileheight;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    tileheight = 0;
    json = nullptr;

    json = Json::readFromFile(file_path);
    tileheight = json["tileheight"];

    EXPECT_EQ(32, tileheight);
}

#if 0
TEST_F(TestJson, cJSONReadMapTileHeight)
{
    cJSON* json;
    cJSON* tileheight;

    const std::string file_path = "test_data/map2_16x16_redone.tmj";

    json = 0;
    tileheight = 0;

    json = Json::readFromFile(file_path);

    tileheight = cJSON_GetObjectItemCaseSensitive(json, "tileheight");

    EXPECT_EQ(32, tileheight->valueint);
}
#endif