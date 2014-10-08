#include <gloperate-osg/OsgRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>

#include <gloperate-osg/OsgKeyboardHandler.h>
#include <gloperate-osg/OsgMouseHandler.h>


using namespace gl;
using namespace gloperate;
namespace gloperate_osg
{


/**
*  @brief
*    Constructor
*/
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
    // Add input slots
    addInput("viewport", m_viewport);

    // [TODO] This should not be necessary, but if off, at the moment the image flickers
    alwaysProcess(true);
}

/**
*  @brief
*    Destructor
*/
OsgRenderStage::~OsgRenderStage()
{
    osg_cleanup();
}

/**
*  @brief
*    Get OSG viewer
*/
osgViewer::Viewer * OsgRenderStage::viewer() const
{
    return m_viewer;
}

/**
*  @brief
*    Get OSG scene
*/
osg::Node * OsgRenderStage::scene() const
{
    return m_scene;
}

/**
*  @brief
*    Set OSG scene
*/
void OsgRenderStage::setScene(osg::Node * scene)
{
    osg_setScene(scene);
}

/**
*  @brief
*    Create keyboard handler to control the wrapped OSG scene
*/
OsgKeyboardHandler * OsgRenderStage::createKeyboardHandler() const
{
    if (m_embedded) {
        return new OsgKeyboardHandler(m_embedded);
    } else {
        return nullptr;
    }
}

/**
*  @brief
*    Create mouse handler to control the wrapped OSG scene
*/
OsgMouseHandler * OsgRenderStage::createMouseHandler() const
{
    if (m_embedded) {
        return new OsgMouseHandler(m_embedded);
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


} // namespace gloperate_osg
