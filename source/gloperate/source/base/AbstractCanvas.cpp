
#include <gloperate/base/AbstractCanvas.h>

#include <cppassist/logging/logging.h>

#include <gloperate/base/Environment.h>
#include <cppassist/memory/make_unique.h>
#include <gloperate/tools/AbstractVideoExporter.h>
#include <gloperate/tools/ImageExporter.h>


namespace
{
    auto s_nextCanvasId = size_t(0);
}


namespace gloperate
{


AbstractCanvas::AbstractCanvas(Environment * environment)
: cppexpose::Object("canvas" + std::to_string(s_nextCanvasId++))
, m_environment(environment)
, m_openGLContext(nullptr)
, m_imageExporter(nullptr)
, m_videoExporter(nullptr)
, m_requestImage(false)
, m_requestVideo(false)
, m_asyncVideoExportOn(false)
, m_asyncVideoFinalize(false)
{
    addFunction("exportImage",          this, &AbstractCanvas::exportImage);
    addFunction("setVideoTarget",       this, &AbstractCanvas::setVideoTarget);
    addFunction("exportVideo",          this, &AbstractCanvas::exportVideo);
    addFunction("toggleVideoExport",    this, &AbstractCanvas::toggleVideoExport);
    addFunction("exportProgress",       this, &AbstractCanvas::exportProgress);
    addFunction("videoExporterPlugins", this, &AbstractCanvas::videoExporterPlugins);
}

AbstractCanvas::~AbstractCanvas()
{
}

const Environment * AbstractCanvas::environment() const
{
    return m_environment;
}

Environment * AbstractCanvas::environment()
{
    return m_environment;
}

const AbstractGLContext * AbstractCanvas::openGLContext() const
{
    return m_openGLContext;
}

AbstractGLContext * AbstractCanvas::openGLContext()
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

void AbstractCanvas::exportImage(const std::string & filename, int width, int height, int renderIterations)
{
    // Lazy creation of image exporter
    if (!m_imageExporter)
    {
        m_imageExporter = make_unique<ImageExporter>();
    }

    // Configure image exporter
    m_imageExporter->setTarget(this, filename, width, height, renderIterations);

    // Execute image exporter on next frame
    m_requestImage = true;
}

void AbstractCanvas::setVideoTarget(const cppexpose::Variant & parameters, const std::string & backend)
{
    // Check parameters
    const cppexpose::VariantMap * map = parameters.asMap();
    if (!map) return;

    // Create video exporter backend
    auto component = m_environment->componentManager()->component<AbstractVideoExporter>(backend);
    if (!component) return;

    // Create video exporter
    m_videoExporter = component->createInstance();

    // Configure video exporter
    m_videoExporter->setTarget(this, *map);
}

void AbstractCanvas::exportVideo(const cppexpose::Variant & parameters, const std::string & backend)
{
    setVideoTarget(parameters, backend);

    if (!m_videoExporter)
        return;

    // Execute video exporter on next frame
    m_requestVideo = true;
}

void AbstractCanvas::toggleVideoExport()
{
    if (!m_videoExporter && !m_asyncVideoExportOn)
    {
        cppassist::warning() << "VideoExporter not properly initialized. Call setVideoTarget() before using the video export functionality.";
        return;
    }

    if (m_asyncVideoExportOn)
    {
        m_asyncVideoFinalize = true;
    } else {
        m_asyncVideoExportOn = true;
    }
}

int AbstractCanvas::exportProgress()
{
    if (!m_videoExporter)
    {
        return 0;
    }

    return m_videoExporter->progress();
}

cppexpose::Variant AbstractCanvas::videoExporterPlugins()
{
    cppexpose::VariantArray plugins;

    for (auto component : m_environment->componentManager()->components<gloperate::AbstractVideoExporter>())
    {
        plugins.push_back(cppexpose::Variant(component->name()));
    }

    return cppexpose::Variant(plugins);
}

void AbstractCanvas::render(globjects::Framebuffer * targetFBO)
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

    // Render current frame into video, blit onto screen
    if (m_asyncVideoExportOn)
    {
        if (m_asyncVideoFinalize)
        {
            m_videoExporter->onRender(AbstractVideoExporter::IgnoreContext, targetFBO, true);
            m_asyncVideoExportOn = false;
            m_asyncVideoFinalize = false;
            return;
        }

        m_videoExporter->onRender(AbstractVideoExporter::IgnoreContext, targetFBO);
        return;
    }

    onRender(targetFBO);
}

void AbstractCanvas::onRender(globjects::Framebuffer *)
{ 
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
