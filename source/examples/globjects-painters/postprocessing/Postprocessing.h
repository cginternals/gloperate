
#pragma once


#include <gloperate/pipeline/PipelinePainter.h>

#include "PostprocessingPipeline.h"


class Postprocessing : public gloperate::PipelinePainter
{
public:
    Postprocessing(gloperate::ResourceManager & resourceManager, const std::string & relDataPath);

    bool animation() const;
    void setAnimation(const bool & enabled);


protected:
    PostprocessingPipeline m_pipeline;

    bool m_animation;
};
