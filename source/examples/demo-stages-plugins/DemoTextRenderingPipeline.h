
#pragma once


#include <cppfs/FilePath.h>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>
#include <gloperate/gloperate-version.h>


namespace openll
{
    enum class Alignment : unsigned char;
    enum class LineAnchor : unsigned char;
}


class DemoTextRenderingPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoTextRenderingPipeline, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Text demo"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    gloperate::CanvasInterface canvasInterface;

    Input<cppfs::FilePath> fontFilename;

    Input<std::string> string;
    Input<uint32_t> numChars;

    Input<float> fontSize;

    Input<glm::vec2> origin;
    Input<glm::vec4> margins;

    Input<bool> wordWrap;
    Input<float> lineWidth;

    Input<openll::Alignment> alignment;
    Input<openll::LineAnchor> lineAnchor;
    Input<bool> optimized;


public:
    explicit DemoTextRenderingPipeline(gloperate::Environment * environment, const std::string & name = "");
};
