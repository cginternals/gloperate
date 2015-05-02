
#pragma once

#include <gloperate/base/make_unique.hpp>
#include <gloperate/pipeline/AbstractPipeline.h>

#include "DummyStage.hpp"


using namespace gloperate;

class TestPipeline : public AbstractPipeline
{
public:
    TestPipeline()
    {
        auto stage0 = new DummyStage("", std::vector<std::string>{}, std::vector<std::string>{"output0", "output1"});
        auto stage1 = new DummyStage("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage2 = new DummyStage("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage3 = new DummyStage("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage4 = new DummyStage("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage5 = new DummyStage("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage6 = new DummyStage("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage7 = new DummyStage("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage8 = new DummyStage("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage9 = new DummyStage("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage10 = new DummyStage("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage11 = new DummyStage("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage12 = new DummyStage("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage13 = new DummyStage("", std::vector<std::string>{"input0", "input1", "input2", "input3", "input4"}, std::vector<std::string>{"output0"});
        auto stage14 = new DummyStage("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0", "output1", "output2", "output3", "output4"});
        auto stage15 = new DummyStage("", std::vector<std::string>{"input0", "input1", "input2", "input3", "input4", "input5"}, std::vector<std::string>{"output0"});
        auto stage16 = new DummyStage("", std::vector<std::string>{"input0"}, std::vector<std::string>{});

        stage1->inputs.at("input0") = stage0->outputs.at("output0");

        stage2->inputs.at("input0") = stage1->outputs.at("output0");

        stage3->inputs.at("input0") = stage0->outputs.at("output0");
        stage3->inputs.at("input1") = stage2->outputs.at("output0");

        stage4->inputs.at("input0") = stage3->outputs.at("output0");

        stage5->inputs.at("input0") = stage3->outputs.at("output0");
        stage5->inputs.at("input1") = stage4->outputs.at("output0");

        stage6->inputs.at("input0") = stage3->outputs.at("output0");
        stage6->inputs.at("input1") = stage4->outputs.at("output0");

        stage7->inputs.at("input0") = stage3->outputs.at("output0");
        stage7->inputs.at("input0") = stage4->outputs.at("output0");

        stage8->inputs.at("input0") = stage3->outputs.at("output0");
        stage8->inputs.at("input1") = stage4->outputs.at("output0");
        stage8->inputs.at("input2") = stage5->outputs.at("output0");

        stage9->inputs.at("input0") = stage3->outputs.at("output0");
        stage9->inputs.at("input1") = stage4->outputs.at("output0");
        stage9->inputs.at("input2") = stage6->outputs.at("output0");

        stage10->inputs.at("input0") = stage3->outputs.at("output0");
        stage10->inputs.at("input1") = stage4->outputs.at("output0");
        stage10->inputs.at("input2") = stage7->outputs.at("output0");

        stage11->inputs.at("input0") = stage10->outputs.at("output0");

        stage12->inputs.at("input0") = stage3->outputs.at("output0");
        stage12->inputs.at("input1") = stage4->outputs.at("output0");
        stage12->inputs.at("input2") = stage8->outputs.at("output0");

        stage13->inputs.at("input0") = stage3->outputs.at("output0");
        stage13->inputs.at("input1") = stage4->outputs.at("output0");
        stage13->inputs.at("input2") = stage12->outputs.at("output0");
        stage13->inputs.at("input3") = stage9->outputs.at("output0");
        stage13->inputs.at("input4") = stage11->outputs.at("output0");

        stage14->inputs.at("input0") = stage13->outputs.at("output0");

        stage15->inputs.at("input0") = stage14->outputs.at("output0");
        stage15->inputs.at("input1") = stage14->outputs.at("output1");
        stage15->inputs.at("input2") = stage14->outputs.at("output2");
        stage15->inputs.at("input3") = stage14->outputs.at("output3");
        stage15->inputs.at("input4") = stage14->outputs.at("output4");
        stage15->inputs.at("input5") = stage0->outputs.at("output1");

        stage16->inputs.at("input0") = stage15->outputs.at("output0");

        addStages(
            stage0,
            stage1,
            stage2,
            stage3,
            stage4,
            stage5,
            stage6,
            stage7,
            stage8,
            stage9,
            stage10,
            stage11,
            stage12,
            stage13,
            stage14,
            stage15,
            stage16);
    }

};
