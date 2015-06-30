
#include <gloperate-osg/OsgRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <gloperate-osg/OsgKeyboardHandler.h>
#include <gloperate-osg/OsgMouseHandler.h>


using namespace gl;
using namespace gloperate;


namespace gloperate_osg
{


OsgRenderStage::OsgRenderStage(const std::string & name)
: AbstractStage(name)
, m_viewer(nullptr)
, m_embedded(nullptr)
, m_scene(nullptr)
, m_viewportX(0)
, m_viewportY(0)
, m_viewportW(0)
, m_viewportH(0)
{
    // Register input slots
    addInput        ("viewport",    m_viewport);
    addOptionalInput("virtualTime", m_virtualTime);

    // Register output slots
    addOutput("projectionMatrix", m_projectionMatrix);
    addOutput("viewMatrix",       m_viewMatrix);
}

OsgRenderStage::~OsgRenderStage()
{
    osg_cleanup();
}

osgViewer::Viewer * OsgRenderStage::viewer() const
{
    return m_viewer;
}

osg::Node * OsgRenderStage::scene() const
{
    return m_scene;
}

void OsgRenderStage::loadScene(const std::string & filename)
{
    osg_loadScene(filename);
}

void OsgRenderStage::setScene(osg::Node * scene)
{
    osg_setScene(scene);
}

OsgKeyboardHandler * OsgRenderStage::createKeyboardHandler()
{
    if (m_embedded) {
        return new OsgKeyboardHandler(m_embedded, this);
    } else {
        return nullptr;
    }
}

OsgMouseHandler * OsgRenderStage::createMouseHandler()
{
    if (m_embedded) {
        return new OsgMouseHandler(m_embedded, this);
    } else {
        return nullptr;
    }
}

void OsgRenderStage::initialize()
{
    osg_initialize();
}

void OsgRenderStage::process()
{
    // Draw osg scene
    osg_process();
}

void OsgRenderStage::handleViewportChanged()
{
}

void OsgRenderStage::postOsgRendering()
{
}


} // namespace gloperate_osg
