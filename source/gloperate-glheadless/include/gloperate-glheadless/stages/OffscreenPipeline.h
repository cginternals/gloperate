
#pragma once


#include <gloperate/pipeline/Pipeline.h>

#include <gloperate-glheadless/gloperate-glheadless_api.h>


namespace gloperate_glheadless {


/**
*  @brief
*    Base class for pipelines rendering to an offscreen context
*/
class GLOPERATE_GLHEADLESS_API OffscreenPipeline : public gloperate::Pipeline
{
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
    OffscreenPipeline(gloperate::Environment * environment, const std::string & name);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OffscreenPipeline();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

    // Helper functions
    void createAndSetUpOffscreenContext(gloperate::AbstractGLContext * context);


protected:
    std::unique_ptr<gloperate::AbstractGLContext> m_offscreenContext; ///< The offscreen context
};


} // namespace gloperate_glheadless
