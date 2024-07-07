#if 0

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

#if 0
TEST_F(TestSpritesheet, SpritesheetLoadsExpectedTmjData)
{
}
#endif

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
