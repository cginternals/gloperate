
#include <gloperate-osg/OsgFboRenderStage.h>

#include <osgViewer/Viewer>
#include <osgViewer/Renderer>

#include <osg/Camera>
#include <osg/Texture2D>


namespace gloperate_osg
{


void OsgFboRenderStage::updateFbo_osg()
{
    // Get OSG camera
    osg::Camera * camera = (viewer() ? viewer()->getCamera() : nullptr);
    if (camera && m_viewportW > 0 && m_viewportH > 0) {
        // (Re)create color texture
        osg::Texture2D * colorTextureOsg = new osg::Texture2D;
        colorTextureOsg->setTextureSize(m_viewportW, m_viewportH);
        colorTextureOsg->setInternalFormat(GL_RGBA);
        colorTextureOsg->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
        colorTextureOsg->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
        colorTextureOsg->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        colorTextureOsg->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        m_colorTextureOsg = colorTextureOsg;

        // (Re)create depth texture
        osg::Texture2D * depthTextureOsg = new osg::Texture2D;
        depthTextureOsg->setTextureSize(m_viewportW, m_viewportH);
        depthTextureOsg->setSourceFormat(GL_DEPTH_COMPONENT);
        depthTextureOsg->setInternalFormat(GL_DEPTH_COMPONENT24);
        depthTextureOsg->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
        depthTextureOsg->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
        depthTextureOsg->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        depthTextureOsg->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
        m_depthTextureOsg = depthTextureOsg;

        // Create FBO for camera
        camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        camera->detach(osg::Camera::COLOR_BUFFER0);
        camera->attach(osg::Camera::COLOR_BUFFER0, m_colorTextureOsg);
        camera->detach(osg::Camera::DEPTH_BUFFER);
        camera->attach(osg::Camera::DEPTH_BUFFER,  m_depthTextureOsg);
        camera->setViewport(0, 0, m_viewportW, m_viewportH);

        // Update projection matrix to preserve the aspect ratio
        camera->setProjectionMatrixAsPerspective(30.0f, camera->getViewport()->aspectRatio(), 1.0f, 10000.0f);

        // Make sure the camera FBO is rebuilt
        osgViewer::Renderer * renderer = (osgViewer::Renderer*)camera->getRenderer();
        renderer->getSceneView(0)->getRenderStage()->setCameraRequiresSetUp(true);
        renderer->getSceneView(0)->getRenderStage()->setFrameBufferObject(nullptr);
    }
}

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
