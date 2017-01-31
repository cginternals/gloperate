
#pragma once


#include <cppassist/fs/FilePath.h>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/gloperate-version.h>


namespace gloperate_text
{
enum class Alignment : unsigned char;
enum class LineAnchor : unsigned char;
}


class TextRenderingPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TextRenderingPipeline, gloperate::Stage
      , "RenderStage"   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Text demo"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    gloperate::RenderInterface renderInterface;

    Input<cppassist::FilePath> fontFilename;

    Input<std::string> string;
    Input<uint32_t> numChars;

    Input<float> fontSize;

    Input<glm::vec2> origin;
    Input<glm::vec4> margins;

    Input<bool> wordWrap;
    Input<float> lineWidth;

    Input<gloperate_text::Alignment> alignment;
    Input<gloperate_text::LineAnchor> lineAnchor;
    Input<bool> optimized;


public:
    explicit TextRenderingPipeline(gloperate::Environment * environment, const std::string & name = "");
};
