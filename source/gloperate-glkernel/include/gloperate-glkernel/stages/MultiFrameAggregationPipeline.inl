
#include <gloperate-glkernel/stages/MultiFrameControlStage.h>

namespace gloperate_glkernel
{


template <typename T>
void MultiFrameAggregationPipeline::restartAggregationOn(gloperate::Slot<T> * slot)
{
    slot->valueChanged.connect([this](const T & /*newValue*/){
        m_controlStage->inputValueChanged(nullptr);
    });
}


} // namespace gloperate_glkernel
