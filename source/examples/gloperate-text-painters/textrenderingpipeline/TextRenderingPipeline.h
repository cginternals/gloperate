
#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <reflectionzeug/base/FilePath.h>
//#include <reflectionzeug/base/Color.h>

#include <gloperate/pipeline/AbstractPipeline.h>
#include <gloperate/pipeline/Data.h>


namespace gloperate
{
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractVirtualTimeCapability;
    class ResourceManager;
}

namespace gloperate_text
{
    enum class Alignment : unsigned char;
}


//enum class TextGenerationAlgorithm : unsigned int;

class TextRenderingPipeline : public gloperate::AbstractPipeline
{
public:
    TextRenderingPipeline();
    virtual ~TextRenderingPipeline();

public:
    gloperate::Data<gloperate::ResourceManager *> resourceManager;
    gloperate::Data<reflectionzeug::FilePath> fontFilename;

    //gloperate::Data<TextGenerationAlgorithm> textGenerationAlgorithm;
    gloperate::Data<std::string> string;
    //gloperate::Data<std::uint32_t> length;

    gloperate::Data<gloperate::AbstractVirtualTimeCapability *> time;

    gloperate::Data<float> pixelPerInch;
    gloperate::Data<float> fontSize;

    gloperate::Data<glm::vec2> origin;
    gloperate::Data<glm::vec4> margins;

    gloperate::Data<bool> wordWrap;
    gloperate::Data<float> lineWidth;

    gloperate::Data<gloperate_text::Alignment> alignment;

    gloperate::Data<gloperate::AbstractTargetFramebufferCapability *> targetFBO;
    gloperate::Data<gloperate::AbstractViewportCapability *> viewport;
};
