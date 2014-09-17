/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <glbinding/gl/gl.h>
#include <gloperate-osg/OsgPainter.h>
#include <gloperate/capabilities/ViewportCapability.h>
#include <gloperate/capabilities/TargetFramebufferCapability.h>


using namespace gl;
using namespace gloperate;
namespace gloperate_osg
{


/**
*  @brief
*    Constructor
*/
OsgPainter::OsgPainter(ResourceManager & resourceManager)
: Painter(resourceManager)
, m_viewer(nullptr)
, m_embedded(nullptr)
, m_scene(nullptr)
, m_viewportCapability(new gloperate::ViewportCapability)
, m_targetFramebufferCapability(new gloperate::TargetFramebufferCapability)
{
    // Register capabilities
    addCapability(m_viewportCapability);
    addCapability(m_targetFramebufferCapability);
}

/**
*  @brief
*    Destructor
*/
OsgPainter::~OsgPainter()
{
    osg_cleanup();
}

/**
*  @brief
*    Get OSG scene
*/
osg::Node * OsgPainter::scene() const
{
    return m_scene;
}

/**
*  @brief
*    Set OSG scene
*/
void OsgPainter::setScene(osg::Node * scene)
{
    osg_setScene(scene);
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
