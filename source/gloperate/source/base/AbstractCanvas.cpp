
#include <gloperate/base/AbstractCanvas.h>

#include <globjects/base/baselogging.h>

#include <gloperate/base/Environment.h>


namespace gloperate
{


static int s_nextCanvasId = 0;


AbstractCanvas::AbstractCanvas(Environment * environment)
: cppexpose::Object("canvas" + std::to_string(s_nextCanvasId++))
, m_environment(environment)
, m_openGLContext(nullptr)
{
    addFunction("createVideo", this, &AbstractCanvas::createVideo);
    addFunction("exportImage", this, &AbstractCanvas::exportImage);
    addFunction("exportProgress", this, &AbstractCanvas::exportProgress);
    addFunction("videoExporterPlugins", this, &AbstractCanvas::videoExporterPlugins);

    m_environment->registerCanvas(this);
}

AbstractCanvas::~AbstractCanvas()
{
    m_environment->unregisterCanvas(this);
}

Environment * AbstractCanvas::environment() const
{
    return m_environment;
}

AbstractGLContext * AbstractCanvas::openGLContext() const
{
    return m_openGLContext;
}

void AbstractCanvas::setOpenGLContext(AbstractGLContext * context)
{
    // Deinitialize renderer in old context
    if (m_openGLContext)
    {
        onContextDeinit();

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        m_openGLContext = context;

        onContextInit();
    }
}

glm::vec4 AbstractCanvas::deviceViewport()
{
    return glm::vec4();
}

glm::vec4 AbstractCanvas::virtualViewport()
{
    return glm::vec4();
}

void AbstractCanvas::onUpdate()
{
}

void AbstractCanvas::onContextInit()
{
}

void AbstractCanvas::onContextDeinit()
{
}

void AbstractCanvas::onViewport(const glm::vec4 &, const glm::vec4 &)
{
}

void AbstractCanvas::onBackgroundColor(float, float, float)
{
}

void AbstractCanvas::onRender(globjects::Framebuffer *)
{
}

void AbstractCanvas::onKeyPress(int, int)
{
}

void AbstractCanvas::onKeyRelease(int, int)
{
}

void AbstractCanvas::onMouseMove(const glm::ivec2 &)
{
}

void AbstractCanvas::onMousePress(int, const glm::ivec2 &)
{
}

void AbstractCanvas::onMouseRelease(int, const glm::ivec2 &)
{
}

void AbstractCanvas::onMouseWheel(const glm::vec2 &, const glm::ivec2 &)
{
}

void AbstractCanvas::createVideo(std::string, int, int, int, int, std::string)
{
}

void AbstractCanvas::exportImage(std::string, int, int, int)
{
}

int AbstractCanvas::exportProgress()
{
    return 0;
}

cppexpose::VariantArray AbstractCanvas::videoExporterPlugins()
{
    return cppexpose::VariantArray();
}


} // namespace gloperate
