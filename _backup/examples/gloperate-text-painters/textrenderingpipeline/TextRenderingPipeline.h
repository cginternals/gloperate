
#pragma once

#include <string>
#include <cstdint>

#include <reflectionzeug/base/FilePath.h>
#include <reflectionzeug/base/Color.h>

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
    enum class Encoding : unsigned int;
}


enum class TextGenerationAlgorithm : unsigned int;

class TextRenderingPipeline : public gloperate::AbstractPipeline
{
public:
    TextRenderingPipeline();
    virtual ~TextRenderingPipeline();

public:
    gloperate::Data<gloperate::ResourceManager *> resourceManager;
    gloperate::Data<reflectionzeug::FilePath> fontFilename;

    gloperate::Data<TextGenerationAlgorithm> textGenerationAlgorithm;
    gloperate::Data<std::string> staticText;
    gloperate::Data<std::uint32_t> length;
    gloperate::Data<gloperate_text::Encoding> encoding;

    gloperate::Data<gloperate::AbstractVirtualTimeCapability *> time;
    gloperate::Data<std::uint32_t> xRepeat;
    gloperate::Data<std::uint32_t> yRepeat;
    gloperate::Data<std::uint32_t> jiggleRadius;

    gloperate::Data<gloperate::AbstractTargetFramebufferCapability *> targetFBO;
    gloperate::Data<gloperate::AbstractViewportCapability *> viewport;
    gloperate::Data<reflectionzeug::Color> fontColor;
    gloperate::Data<float> distanceThreshold;
};
