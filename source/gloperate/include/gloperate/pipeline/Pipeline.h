
#pragma once


#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


/**
*  @brief
*    Pipeline
*
*    A pipeline consists of several stages which are executed by the order
*    of their mutual dependencies. The pipeline itself is a stage and can
*    be used in the same way as simple stages, e.g., it can be part of
*    other pipelines.
*
*    The pipeline concept is that of a pull-pipeline:
*    - Output data can be marked as 'required'. Any stage will try to produce
*      all required output data, so if a stage has an output that is required
*      and invalid, it will be executed in order to produce that output.
*    - If output data is 'required', all input slots from that stage are also
*      marked as 'required'. This determines which stages will be executed
*      on a pipeline.
*    - When input data has changed, it will invalidate the respective outputs
*      of its stage, so any change of input data will propagate through the
*      pipeline immediately and invalidate all outputs that, directly or
*      indirectly, depend on that input.
*/
class GLOPERATE_API Pipeline : public Stage
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] parent
    *    Parent stage
    */
    Pipeline(ViewerContext * viewerContext, Stage * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Pipeline();
};


} // namespace gloperate
