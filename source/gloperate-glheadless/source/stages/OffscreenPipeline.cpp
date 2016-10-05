
#include <gloperate-glheadless/stages/OffscreenPipeline.h>

#include <gloperate/base/GLContextFormat.h>
#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-glheadless/base/GLContextFactory.h>
#include <gloperate-glheadless/util/ScopedContext.h>


using namespace gloperate;


namespace gloperate_glheadless {


OffscreenPipeline::OffscreenPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
{
}


OffscreenPipeline::~OffscreenPipeline()
{
}


void OffscreenPipeline::onContextInit(AbstractGLContext * context)
{
    // Create offscreen context
    createAndSetUpOffscreenContext(context);

    // Activate offscreen context
    ScopedContext contextSwitch(context, m_offscreenContext.get());

    // Init child stages with offscreen context
    Pipeline::onContextInit(m_offscreenContext.get());
}


void OffscreenPipeline::onContextDeinit(AbstractGLContext * context)
{
    {
        // Activate offscreen context
        ScopedContext contextSwitch(context, m_offscreenContext.get());

        // Deinit child stages with offscreen context
        Pipeline::onContextDeinit(m_offscreenContext.get());
    }

    // Delete offscreen context
    m_offscreenContext = nullptr;
}


void OffscreenPipeline::onProcess(AbstractGLContext * context)
{
    // Activate offscreen context
    ScopedContext contextSwitch(context, m_offscreenContext.get());

    // Process child stages with offscreen context
    Pipeline::onProcess(m_offscreenContext.get());
}


void OffscreenPipeline::createAndSetUpOffscreenContext(AbstractGLContext * context)
{
    // [TODO] context format parameterization

    // Create context format
    GLContextFormat format;
    format.setVersion(3, 2);
    format.setProfile(GLContextFormat::Profile::Core);

    // Create offscreen context
    GLContextFactory factory;
    factory.setSharedContext(context);
    m_offscreenContext.reset(factory.createBestContext(format));
}


} // namespace gloperate_glheadless
