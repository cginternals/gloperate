
#include <gmock/gmock.h>

#include <iostream>

#include "TestPipeline.hpp"


using namespace gloperate;


class AbstractPipeline_test : public testing::Test
{
public:
    AbstractPipeline_test()
    {
    }
};


TEST_F(AbstractPipeline_test, PipelineIsSortable)
{
    TestPipeline pipeline;
    pipeline.initialize();
    ASSERT_TRUE(pipeline.isInitialized());
}
