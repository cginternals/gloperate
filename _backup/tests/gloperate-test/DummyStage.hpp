
#pragma once

#include <map>
#include <vector>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/InputSlot.h>


using namespace gloperate;


class DummyStage : public AbstractStage
{
public:
    DummyStage(
        const std::string & name,
        const std::vector<std::string> & inputNames,
        const std::vector<std::string> & outputNames)
    :   AbstractStage(name)
    {
        for (const auto & inputName : inputNames)
        {
            addInput(name + "_" + inputName, inputs[inputName]);
        }

        for (const auto & outputName : outputNames)
        {
            addOutput(name + "_" + outputName, outputs[outputName]);
        }
    }
    
    virtual void process() override
    {
    }


public:
    std::map<std::string, InputSlot<int>> inputs;
    std::map<std::string, Data<int>> outputs;
};
