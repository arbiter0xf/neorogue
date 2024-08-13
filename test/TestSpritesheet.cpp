#include "gtest/gtest.h"

#include "GraphicsUtil.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

class TestSpritesheet : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TestSpritesheet, SpritesheetGetsFirstgid)
{
    spritesheet_pool spritesheetPool;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");

    GraphicsUtil::loadSpritesheets(spritesheetPool, testMap);

    Spritesheet& testSpritesheet = spritesheetPool[0];

    for (Spritesheet& s : spritesheetPool) {
        if (0 == s.getName().compare("dc-misc-collection-spritesheet")) {
            testSpritesheet = s;
            break;
        }
    }

    EXPECT_EQ(testSpritesheet.getTiledFirstgid(), 560);
}

TEST_F(TestSpritesheet, MonSpritesheetContainsOneLayer)
{
    cJSON* tmj;
    cJSON* layers_array;
    spritesheet_pool spritesheet_pool;

    Map test_map = Map("test_data/map2_16x16_redone.tmj");

    tmj = 0;
    layers_array = 0;

    GraphicsUtil::loadSpritesheets(spritesheet_pool, test_map);

    Spritesheet& test_spritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.getName().compare("dc-mon-collection-spritesheet")) {
            test_spritesheet = s;
            break;
        }
    }

    tmj = test_spritesheet.getJson();

    layers_array = cJSON_GetObjectItemCaseSensitive(tmj, "layers");

    EXPECT_EQ(cJSON_GetArraySize(layers_array), 1);
}

TEST_F(TestSpritesheet, MonSpritesheetDataIndex82Gives83)
{
    cJSON* tmj;
    cJSON* data_array;
    cJSON* data_item;
    cJSON* layer;
    cJSON* layers_array;
    spritesheet_pool spritesheet_pool;

    Map test_map = Map("test_data/map2_16x16_redone.tmj");

    tmj = 0;
    layer = 0;
    data_item = 0;
    data_array = 0;
    layers_array = 0;

    GraphicsUtil::loadSpritesheets(spritesheet_pool, test_map);

    Spritesheet& test_spritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.getName().compare("dc-mon-collection-spritesheet")) {
            test_spritesheet = s;
            break;
        }
    }

    tmj = test_spritesheet.getJson();
    layers_array = cJSON_GetObjectItemCaseSensitive(tmj, "layers");
    layer = cJSON_GetArrayItem(layers_array, 0);
    data_array = cJSON_GetObjectItemCaseSensitive(layer, "data");
    data_item = cJSON_GetArrayItem(data_array, 82);

    EXPECT_EQ(data_item->valueint, 83);
}

#if 0
TEST_F(TestSpritesheet, SpritesheetLoadsExpectedTmjDataBoostlib)
{
    spritesheet_pool spritesheetPool;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");

    GraphicsUtil::loadSpritesheets(spritesheetPool, testMap);

    Spritesheet& testSpritesheet = spritesheetPool[0];

    for (Spritesheet& s : spritesheetPool) {
        if (0 == s.getName().compare("dc-mon-collection-spritesheet")) {
            testSpritesheet = s;
            break;
        }
    }

    boost::json::value tmj = testSpritesheet.getJson();

    boost::json::value layersArray = tmj
        .get_object()
        .find("layers")
        ->value();

    if (layersArray.as_array().size() > 1) {
        throw std::runtime_error("More than 1 layer in a spritesheet JSON");
    }

    boost::json::value layerData = layersArray.as_array()[0]
        .get_object()
        .find("data")
        ->value();

    EXPECT_EQ(layerData.as_array()[82].as_int64(), 83);
}
#endif
