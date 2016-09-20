
#include <gloperate/stages/multiframe/MultiFrameControlStage.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(MultiFrameControlStage, gloperate::Stage)


MultiFrameControlStage::MultiFrameControlStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, m_currentFrame(1)
, frameNumber("frameNumber", this)
, currentFrame("currentFrame", this)
, aggregationFactor("aggregationFactor", this)
{
}

MultiFrameControlStage::~MultiFrameControlStage()
{
}

void MultiFrameControlStage::onProcess(AbstractGLContext *)
{
    // TODO: Mechanism to restart aggregation

    currentFrame.setValue(m_currentFrame);
    aggregationFactor.setValue(1.0f/m_currentFrame);

    m_currentFrame++;
}


} // namespace gloperate
