
#include <gloperate-glkernel/stages/MultiFrameControlStage.h>

#include <gloperate/base/Environment.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(MultiFrameControlStage, gloperate::Stage)


MultiFrameControlStage::MultiFrameControlStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, timeDelta("timeDelta", this)
, frameNumber("frameNumber", this)
, multiFrameCount("multiFrameCount", this)
, viewport("viewport", this)
, currentFrame("currentFrame", this)
, aggregationFactor("aggregationFactor", this)
, m_currentFrame(0)
{
}

MultiFrameControlStage::~MultiFrameControlStage()
{
}

void MultiFrameControlStage::onProcess()
{
    if (m_currentFrame < *multiFrameCount)
    {
        currentFrame.setValue(m_currentFrame);
        aggregationFactor.setValue(1.0f/(m_currentFrame+1));
        ++m_currentFrame;
    }
    else
    {
        aggregationFactor.setValue(0.0f);
    }
}

void MultiFrameControlStage::onInputValueChanged(gloperate::AbstractSlot * slot)
{
    if (slot != &frameNumber && slot != &timeDelta)
    {
        m_currentFrame = 0;
    }

    Stage::onInputValueChanged(slot);
}


} // namespace gloperate_glkernel
