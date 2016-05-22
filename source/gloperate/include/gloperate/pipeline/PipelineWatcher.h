
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


class PipelineEvent;


/**
*  @brief
*    Class to observe all events on a pipeline or stage
*
*    The PipelineWatcher can be applied to a pipeline. It will then
*    receive all important events on that pipeline, such as adding
*    or removing stages, changing input and output connections, and
*    changing data values. When applied to a pipeline, the watcher
*    will receive all events of the pipeline and all of its children.
*
*    If you are interested in specific events, such as a single slot
*    on a pipeline or stage, you should connect to the signals
*    of that specific item. However, if you need the events of
*    an entire pipeline hierarchy, use this class.
*/
class GLOPERATE_API PipelineWatcher
{
public:
    /**
    *  @brief
    *    Constructor
    */
    PipelineWatcher();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PipelineWatcher();

    /**
    *  @brief
    *    Called when a pipeline event has occured
    *
    *  @param[in] event
    *    Pipeline event
    */
    virtual void onPipelineEvent(const PipelineEvent & event);
};


} // namespace cppexpose
