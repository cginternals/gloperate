
#include <gloperate/base/AbstractCanvas.h>

#include <globjects/base/baselogging.h>

#include <gloperate/base/Environment.h>
#include <gloperate/tools/AbstractVideoExporter.h>
#include <gloperate/tools/ImageExporter.h>


namespace gloperate
{


static int s_nextCanvasId = 0;


AbstractCanvas::AbstractCanvas(Environment * environment)
: cppexpose::Object("canvas" + std::to_string(s_nextCanvasId++))
, m_environment(environment)
, m_openGLContext(nullptr)
, m_imageExporter(nullptr)
, m_videoExporter(nullptr)
, m_requestImage(false)
, m_requestVideo(false)
{
    addFunction("exportImage",          this, &AbstractCanvas::exportImage);
    addFunction("exportVideo",          this, &AbstractCanvas::exportVideo);
    addFunction("exportProgress",       this, &AbstractCanvas::exportProgress);
    addFunction("videoExporterPlugins", this, &AbstractCanvas::videoExporterPlugins);

    m_environment->registerCanvas(this);
}

AbstractCanvas::~AbstractCanvas()
{
    m_environment->unregisterCanvas(this);

    if (m_imageExporter) {
        delete m_imageExporter;
    }

    if (m_videoExporter) {
        delete m_videoExporter;
    }
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

void AbstractCanvas::exportImage(std::string filename, int width, int height, int renderIterations)
{
    // Lazy creation of image exporter
    if (!m_imageExporter)
    {
        m_imageExporter = new ImageExporter;
    }

    // Configure image exporter
    m_imageExporter->setTarget(this, filename, width, height, renderIterations);

    // Execute image exporter on next frame
    m_requestImage = true;
}

void AbstractCanvas::exportVideo(std::string filename, int width, int height, int fps, int seconds, std::string backend)
{
    // Create video exporter backend
    auto component = m_environment->componentManager()->component<AbstractVideoExporter>(backend);
    if (!component) return;

    // Create video exporter
    m_videoExporter = component->createInstance();

    // Configure video exporter
    m_videoExporter->setTarget(this, filename, width, height, fps, seconds);

    // Execute video exporter on next frame
    m_requestVideo = true;
}

int AbstractCanvas::exportProgress()
{
    if (!m_videoExporter)
    {
        return 0;
    }

    return m_videoExporter->progress();
}

cppexpose::VariantArray AbstractCanvas::videoExporterPlugins()
{
    cppexpose::VariantArray plugins;

    for (auto component : m_environment->componentManager()->components<gloperate::AbstractVideoExporter>())
    {
        plugins.push_back(cppexpose::Variant(component->name()));
    }

    return plugins;
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

void AbstractCanvas::onSaveViewport()
{
}

void AbstractCanvas::onResetViewport()
{
}

void AbstractCanvas::onBackgroundColor(float, float, float)
{
}

void AbstractCanvas::onRender(globjects::Framebuffer *)
{
    // In certain viewers, e.g. QML, differents threads for UI and rendering are
    // used. This makes it necessary to postpone any export functionality
    // until the next render call, to make sure that they are executed from within
    // the render thread.

    // Perform image export
    if (m_requestImage)
    {
        m_requestImage = false;

        m_imageExporter->save(ImageExporter::IgnoreContext);
    }

    // Perform video export
    if (m_requestVideo)
    {
        m_requestVideo = false;

        m_videoExporter->createVideo(AbstractVideoExporter::IgnoreContext,
            [this] (int, int)
            {
                this->wakeup();
            }
        );
    }
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


} // namespace gloperate
