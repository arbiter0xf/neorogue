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
    spritesheet_pool spritesheet_pool;
    Map testMap = Map("test_data/map2_16x16_redone.tmj");

    GraphicsUtil::loadSpritesheets(spritesheet_pool, testMap);

    Spritesheet& testSpritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.getName().compare("dc-misc-collection-spritesheet")) {
            testSpritesheet = s;
            break;
        }
    }

    EXPECT_EQ(testSpritesheet.getTiledFirstgid(), 560);
}

TEST_F(TestSpritesheet, MonSpritesheetContainsOneLayer)
{
    nlohmann::json tmj;

    spritesheet_pool spritesheet_pool;

    Map test_map = Map("test_data/map2_16x16_redone.tmj");

    GraphicsUtil::loadSpritesheets(spritesheet_pool, test_map);

    Spritesheet& test_spritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.getName().compare("dc-mon-collection-spritesheet")) {
            test_spritesheet = s;
            break;
        }
    }

    tmj = test_spritesheet.getJson();

    auto layers_array = tmj["layers"];

    EXPECT_EQ(layers_array.size(), 1);
}

TEST_F(TestSpritesheet, MonSpritesheetDataIndex82Gives83)
{
    nlohmann::json tmj;
    nlohmann::json layer;

    spritesheet_pool spritesheet_pool;

    Map test_map = Map("test_data/map2_16x16_redone.tmj");

    tmj = {};

    GraphicsUtil::loadSpritesheets(spritesheet_pool, test_map);

    Spritesheet& test_spritesheet = spritesheet_pool[0];

    for (Spritesheet& s : spritesheet_pool) {
        if (0 == s.getName().compare("dc-mon-collection-spritesheet")) {
            test_spritesheet = s;
            break;
        }
    }

    tmj = test_spritesheet.getJson();
    auto layers_array = tmj["layers"];
    layer = layers_array[0];
    auto data_array = layer["data"];
    auto data_item = data_array[82];

    EXPECT_EQ(data_item, 83);
}
