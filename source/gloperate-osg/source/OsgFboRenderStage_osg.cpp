#include <gloperate-osg/OsgFboRenderStage.h>

#include <osgViewer/Viewer>
#include <osgViewer/Renderer>
#include <osg/Camera>
#include <osg/Texture2D>


namespace gloperate_osg
{


/**
*  @brief
*    Called to setup target FBO for rendering the OSG scene
*/
void OsgFboRenderStage::updateFbo_osg()
{
    // Get OSG camera
    osg::Camera * camera = (viewer() ? viewer()->getCamera() : nullptr);
    if (camera && m_viewportW > 0 && m_viewportH > 0) {
        // Re-create color texture
        osg::Texture2D * colorTextureOsg = new osg::Texture2D;
        colorTextureOsg->setTextureSize(m_viewportW, m_viewportH);
        colorTextureOsg->setInternalFormat(GL_RGBA);
        colorTextureOsg->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
        colorTextureOsg->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
        colorTextureOsg->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        colorTextureOsg->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        colorTextureOsg->apply(*m_embedded->getState()); // Make sure that the texture is actually generated
        m_colorTextureOsg = colorTextureOsg;

        // Create FBO for camera
        camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        camera->detach(osg::Camera::COLOR_BUFFER0);
        camera->attach(osg::Camera::COLOR_BUFFER0, m_colorTextureOsg);
        camera->setViewport(0, 0, m_viewportW, m_viewportH);

        // Make sure the camera FBO is rebuilt
        osgViewer::Renderer * renderer = (osgViewer::Renderer*)camera->getRenderer();
        renderer->getSceneView(0)->getRenderStage()->setCameraRequiresSetUp(true);
        renderer->getSceneView(0)->getRenderStage()->setFrameBufferObject(nullptr);
    }
}

/**
*  @brief
*    Get texture ID of a given OSG texture
*/
unsigned int OsgFboRenderStage::getOsgTextureId(const osg::Texture * texture) const
{
    // Check if everything is setup correctly
    if (m_embedded && texture) {
        // Get texture ID
        unsigned int contextID = m_embedded->getState()->getContextID();
        return texture->getTextureObject(contextID)->id();
    } else {
        // Return invalid ID on error
        return 0;
    }
}


} // namespace gloperate_osg
