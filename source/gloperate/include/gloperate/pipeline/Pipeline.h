
#pragma once


#include <vector>
#include <unordered_map>
#include <string>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


/**
*  @brief
*    Pipeline
*
*    A pipeline consists of several stages which are executed by the order
*    of their mutual dependencies. The pipeline itself is a stage and can
*    be used in the same way as any other stage, i.e., it can be part of
*    other pipelines.
*
*    The pipeline concept is that of a pull-pipeline:
*    - Output data can be marked as 'required'. Any stage has to produce
*      all required output data, so if a stage has an output that is required
*      but invalid, it will be executed in order to produce that output.
*    - By default, if output data is 'required', all input slots from that
*      stage are also marked as 'required'. This determines which stages
*      will be executed on a pipeline.
*    - When input data has changed, the respective outputs of its stage are
*      invalidated, so any change of input data will propagate through the
*      pipeline immediately and invalidate all outputs that, directly or
*      indirectly, depend on that input.
*/
class GLOPERATE_API Pipeline : public Stage
{
    friend class Stage;


public:
    cppexpose::Signal<Stage *> stageAdded;   ///< Called when a new stage has been added
    cppexpose::Signal<Stage *> stageRemoved; ///< Called when a stage has been removed


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    Pipeline(Environment * environment, const std::string & name = "Pipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Pipeline();

    /**
    *  @brief
    *    Get stages
    *
    *  @return
    *    List of stages in the pipeline
    */
    const std::vector<Stage *> stages() const;

    /**
    *  @brief
    *    Get stage by name
    *
    *  @param[in] name
    *    Name of stage
    *
    *  @return
    *    Stage (can be null)
    */
    Stage * stage(const std::string & name) const;

    /**
    *  @brief
    *    Add stage
    *
    *  @param[in] stage
    *    Stage (must NOT be null!)
    *  @param[in] ownership
    *    Property ownership
    */
    void addStage(Stage * stage, cppexpose::PropertyOwnership ownership = cppexpose::PropertyOwnership::Parent);

    /**
    *  @brief
    *    Remove stage
    *
    *  @param[in] stage
    *    Stage (must NOT be null!)
    *
    *  @return
    *    'true' if the stage was removed, else 'false'
    *
    *  If the stage is not part of the pipeline, nothing happens
    */
    bool removeStage(Stage * stage);

    /**
    *  @brief
    *    Remove and destroy stage
    *
    *  @param[in] stage
    *    Stage (must NOT be null!)
    *
    *  @return
    *    'true' if the stage was removed and destroyed, else 'false'
    *
    *  If the stage is not part of the pipeline, nothing happens
    */
    bool destroyStage(Stage * stage);

    /**
    *  @brief
    *    Invalidate sorted stage order
    *
    *  @remarks
    *    The stages are resorted upon next usage.
    */
    void invalidateStageOrder();

    // Virtual Stage interface
    virtual bool isPipeline() const override;


protected:
    /**
    *  @brief
    *    Sort stages by their dependencies
    */
    void sortStages();

    /**
    *  @brief
    *    Get a slot of this pipeline or a subpipeline
    *
    *  @param[in] path
    *    Path to the slot from this pipeline. Can contain the name of this pipeline as first element.
    *
    *  @return
    *    Slot, nullptr if not found
    */
    AbstractSlot * getSlot(const std::string & path);

    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;
    virtual void onInputValueChanged(AbstractSlot * slot) override;
    virtual void onOutputRequiredChanged(AbstractSlot * slot) override;


protected:
    // Scripting functions
    virtual cppexpose::Variant scr_getDescription() override;
    std::string scr_createStage(const std::string & className, const std::string & name);
    void scr_removeStage(const std::string & name);
    void scr_createConnection(const std::string & from, const std::string & to);
    void scr_removeConnection(const std::string & to);


protected:
    std::vector<Stage *>                     m_stages;    ///< List of topologically sorted stages in the pipeline
    std::unordered_map<std::string, Stage *> m_stagesMap; ///< Map of names -> stages
    bool                                     m_sorted;    ///< Have the stages of the pipeline already been sorted?
};


} // namespace gloperate
