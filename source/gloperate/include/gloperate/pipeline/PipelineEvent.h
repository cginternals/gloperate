
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Stage;
class Pipeline;
class AbstractSlot;


/**
*  @brief
*    Pipeline event
*
*    This class describes an event that has occured in a pipeline.
*    It is propagated through the pipeline and can be handled
*    by overriding Stage::onPipelineEvent(), or by adding a
*    PipelineWatcher to a pipeline or stage.
*/
class GLOPERATE_API PipelineEvent
{
public:
    /**
    *  @brief
    *    Event type
    */
    enum Type
    {
        StageAdded = 1
      , StageRemoved
      , InputAdded
      , InputRemoved
      , ParameterAdded
      , ParameterRemoved
      , OutputAdded
      , OutputRemoved
      , ProxyOutputAdded
      , ProxyOutputRemoved
      , ValueChanged
    };


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Type of the event
    *  @param[in] stage
    *    Stage on which the event occured
    */
    PipelineEvent(Type type, Stage * stage);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Type of the event
    *  @param[in] stage
    *    Stage on which the event occured
    *  @param[in] slot
    *    Slot of the stage that is the subject of the event
    */
    PipelineEvent(Type type, Stage * stage, AbstractSlot * slot);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Type of the event
    *  @param[in] pipeline
    *    Pipeline on which the event occured
    */
    PipelineEvent(Type type, Pipeline * pipeline);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Type of the event
    *  @param[in] pipeline
    *    Pipeline on which the event occured
    *  @param[in] subStage
    *    Sub-stage of the pipeline that is the subject of the event
    */
    PipelineEvent(Type type, Pipeline * pipeline, Stage * subStage);

    /**
    *  @brief
    *    Destructor
    */
    ~PipelineEvent();

    /**
    *  @brief
    *    Get event type
    *
    *  @return
    *    Type of the event
    */
    Type type() const;

    /**
    *  @brief
    *    Get stage
    *
    *  @return
    *    Stage on which the event occured
    */
    Stage * stage() const;

    /**
    *  @brief
    *    Get pipeline
    *
    *  @return
    *    Pipeline on which the event occured
    */
    Pipeline * pipeline() const;

    /**
    *  @brief
    *    Get sub-stage
    *
    *  @return
    *    Sub-stage of the pipeline that is the subject of the event
    */
    Stage * subStage() const;

    /**
    *  @brief
    *    Get slot
    *
    *  @return
    *    Slot of the stage that is the subject of the event
    */
    AbstractSlot * slot() const;


protected:
    Type           m_type;     ///< Type of the event
    Stage        * m_stage;    ///< Stage on which the event occured
    Pipeline     * m_pipeline; ///< Pipeline on which the event occured
    Stage        * m_subStage; ///< Sub-stage of the pipeline that is the subject of the event
    AbstractSlot * m_slot;     ///< Slot of the stage that is the subject of the event
};


} // namespace cppexpose
