
#include <gloperate-osg/OsgPainter.h>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <gloperate/base/make_unique.hpp>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/InputCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>


using namespace gl;
using namespace gloperate;


namespace gloperate_osg
{


OsgPainter::OsgPainter(const std::string & name, ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: Painter(name, resourceManager, moduleInfo)
, m_viewer(nullptr)
, m_embedded(nullptr)
, m_scene(nullptr)
, m_viewportX(0)
, m_viewportY(0)
, m_viewportWidth(0)
, m_viewportHeight(0)
{
    // Register capabilities
    m_viewportCapability = addCapability(new gloperate::ViewportCapability);
    m_targetFramebufferCapability = addCapability(new gloperate::TargetFramebufferCapability);
    m_inputCapability = addCapability(new gloperate::InputCapability);
    m_virtualTimeCapability = addCapability(new gloperate::VirtualTimeCapability);
}

OsgPainter::~OsgPainter()
{
    osg_cleanup();
}

osgViewer::Viewer * OsgPainter::viewer() const
{
    return m_viewer;
}

osg::Node * OsgPainter::scene() const
{
    return m_scene;
}

void OsgPainter::setScene(osg::Node * scene)
{
    osg_setScene(scene);
}

void OsgPainter::loadScene(const std::string & filename)
{
    osg_loadScene(filename);
}

void OsgPainter::onInitialize()
{
    osg_onInitialize();
}

void OsgPainter::onPaint()
{
    // Get framebuffer to render into
    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();
    if (!fbo) {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    // Bind framebuffer
    fbo->bind(GL_FRAMEBUFFER);

    // Draw osg scene
    osg_onPaint();
}


} // namespace gloperate_osg
