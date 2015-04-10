
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
        auto stage0 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{}, std::vector<std::string>{"output0", "output1"});
        auto stage1 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage2 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage3 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage4 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage5 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage6 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage7 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1"}, std::vector<std::string>{"output0"});
        auto stage8 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage9 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage10 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage11 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0"});
        auto stage12 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1", "input2"}, std::vector<std::string>{"output0"});
        auto stage13 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1", "input2", "input3", "input4"}, std::vector<std::string>{"output0"});
        auto stage14 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0"}, std::vector<std::string>{"output0", "output1", "output2", "output3", "output4"});
        auto stage15 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0", "input1", "input2", "input3", "input4", "input5"}, std::vector<std::string>{"output0"});
        auto stage16 = gloperate::make_unique<DummyStage>("", std::vector<std::string>{"input0"}, std::vector<std::string>{});

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
            std::move(stage0),
            std::move(stage1),
            std::move(stage2),
            std::move(stage3),
            std::move(stage4),
            std::move(stage5),
            std::move(stage6),
            std::move(stage7),
            std::move(stage8),
            std::move(stage9),
            std::move(stage10),
            std::move(stage11),
            std::move(stage12),
            std::move(stage13),
            std::move(stage14),
            std::move(stage15),
            std::move(stage16));
    }

};
