
#pragma once


#include <gloperate/pipeline/PipelinePainter.h>

#include "TextRenderingPipeline.h"


class TextRenderingPainter : public gloperate::PipelinePainter
{
public:
    TextRenderingPainter(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo);

protected:
    TextRenderingPipeline m_pipeline;
};
