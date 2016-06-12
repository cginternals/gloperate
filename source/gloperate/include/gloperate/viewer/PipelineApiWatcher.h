
#pragma once


#include <cppexpose/variant/Variant.h>
#include <cppexpose/function/Function.h>

#include <gloperate/pipeline/PipelineWatcher.h>


namespace gloperate
{


// [TODO] Remove the entire class, access pipeline through cppexpose object hierarchy


/**
*  @brief
*    Pipeline watcher for scripting
*/
class GLOPERATE_API PipelineApiWatcher : public PipelineWatcher
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] func
    *    Callback function for pipeline events
    */
    PipelineApiWatcher(const cppexpose::Variant & func);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PipelineApiWatcher();

    // Virtual PipelineWatcher interface
    virtual void onPipelineEvent(const PipelineEvent & event) override;


protected:
    cppexpose::Function m_func; ///< Callback function for pipeline events
};


} // namespace gloperate
