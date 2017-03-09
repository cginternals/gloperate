
#include <gloperate-glkernel/stages/MultiFrameControlStage.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(MultiFrameControlStage, gloperate::Stage)


MultiFrameControlStage::MultiFrameControlStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, frameNumber("frameNumber", this)
, multiFrameCount("multiFrameCount", this)
, viewport("viewport", this)
, currentFrame("currentFrame", this)
, aggregationFactor("aggregationFactor", this)
, m_currentFrame(0)
{
    viewport.valueChanged.connect([this](const glm::vec4 &){
        m_currentFrame = 0;
    });
}

MultiFrameControlStage::~MultiFrameControlStage()
{
}

void MultiFrameControlStage::onProcess(AbstractGLContext *)
{
    m_currentFrame++;

    if (m_currentFrame < *multiFrameCount)
    {
        currentFrame.setValue(m_currentFrame);
        aggregationFactor.setValue(1.0f/m_currentFrame);
    }
}


} // namespace gloperate
