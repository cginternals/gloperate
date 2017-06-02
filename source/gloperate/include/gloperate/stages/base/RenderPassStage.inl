
#pragma once


#include <globjects/Program.h>

#include <gloperate/rendering/RenderPass.h>


namespace gloperate
{


template <typename T>
Input<T> & RenderPassStage::createNewUniformInput(const std::string & name, const T & defaultValue)
{
    auto newInput = createInput(name, defaultValue);

    uniformSetters[name] = [this, name, newInput](){
        if (newInput->isConnected())
        {
            m_renderPass->program()->setUniform<T>(name, **newInput);
        }
    };

    return (*newInput);
}


} // namespace gloperate
