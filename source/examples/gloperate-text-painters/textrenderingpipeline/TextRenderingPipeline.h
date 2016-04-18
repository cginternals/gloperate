
#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <reflectionzeug/base/FilePath.h>
//#include <reflectionzeug/base/Color.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/pipeline/AbstractPipeline.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/resources/ResourceManager.h>

namespace gloperate
{
    class AbstractVirtualTimeCapability;
}

namespace gloperate_text
{
    enum class Alignment : unsigned char;
    enum class LineAnchor : unsigned char;
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
    gloperate::Data<uint32_t> numChars;
    //gloperate::Data<std::uint32_t> length;

    gloperate::Data<gloperate::AbstractVirtualTimeCapability *> time;

    gloperate::Data<float> pixelPerInch;
    gloperate::Data<float> fontSize;

    gloperate::Data<glm::vec2> origin;
    gloperate::Data<glm::vec4> margins;

    gloperate::Data<bool> wordWrap;
    gloperate::Data<float> lineWidth;

    gloperate::Data<gloperate_text::Alignment> alignment;
    gloperate::Data<gloperate_text::LineAnchor> lineAnchor;
    gloperate::Data<bool> optimized;

    gloperate::Data<gloperate::AbstractTargetFramebufferCapability *> targetFBO;
    gloperate::Data<gloperate::AbstractViewportCapability *> viewport;
};
