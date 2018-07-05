
namespace gloperate_glkernel
{


template<typename T>
gloperate::Input<T> * MultiFrameAggregationPipeline::restartAggregationOn(gloperate::Slot<T> * slot)
{
    return m_controlStage->createInput(slot->name()) << *slot;
}


} // namespace gloperate_glkernel
