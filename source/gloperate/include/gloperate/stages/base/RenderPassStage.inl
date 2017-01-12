
#pragma once


#include <gloperate/stages/base/RenderPassStage.h>


namespace gloperate
{


template <typename T>
Input<T> & RenderPassStage::createNewUniformInput(const std::string & name, const T & defaultValue)
{
    auto newInput = createInput(name, defaultValue);

    uniformSetters[name] = [this, name, newInput](){
        if (newInput->isConnected())
        {
            (*renderPass)->program()->setUniform<T>(name, **newInput);
        }
    };

    return (*newInput);
}


} // namespace gloperate
