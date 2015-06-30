
#include <gmock/gmock.h>

#include "DummyStage.hpp"


using namespace gloperate;


class AbstractStage_test : public testing::Test
{
public:
    AbstractStage_test()
    :   stage0{"stage0", {}, { "output0" }}
    ,   stage1{"stage1", { "input0" }, { "output0" }}
    ,   stage2{"stage2", { "input0" }, {}}
    {
        stage1.inputs["input0"] = stage0.outputs["output0"];
        stage2.inputs["input0"] = stage1.outputs["output0"];
    }


protected:
    DummyStage stage0;
    DummyStage stage1;
    DummyStage stage2;
};


TEST_F(AbstractStage_test, Stage1RequiresStage0NonRecursive)
{
    ASSERT_TRUE(stage1.requires(&stage0, false));
}

TEST_F(AbstractStage_test, Stage2RequiresStage1NonRecursive)
{
    ASSERT_TRUE(stage2.requires(&stage1, false));
}

TEST_F(AbstractStage_test, Stage2DoesntRequireStage0NonRecursive)
{
    ASSERT_FALSE(stage2.requires(&stage0, false));
}

TEST_F(AbstractStage_test, Stage2DoesntRequireStage0Recursive)
{
    ASSERT_TRUE(stage2.requires(&stage0, true));
}
