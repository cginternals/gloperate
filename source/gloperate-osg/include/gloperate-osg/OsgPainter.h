#pragma once


// Do not include any osg headers OR gloperate headers that pull in globjects/glbinding,
// because that would break the separation between glbinding and gl.h.
// Use only pointers to OSG objects, call ref/unref to maintain memory management,
// and separate all calls to osg in osg_ functions implemented inside OsgPainter_osg.cpp.


#include <gloperate/Painter.h>
#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer {
    class Viewer;
    class GraphicsWindowEmbedded;
}
namespace osg {
    class Node;
}
namespace gloperate {
    class ViewportCapability;
    class TargetFramebufferCapability;
    class InputCapability;
    class VirtualTimeCapability;
}


namespace gloperate_osg
{


class GLOPERATE_OSG_API OsgPainter : public gloperate::Painter
{


public:
    OsgPainter(gloperate::ResourceManager & resourceManager);
    virtual ~OsgPainter();

    osgViewer::Viewer * viewer() const;
    osg::Node * scene() const;
    void setScene(osg::Node * scene);


protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;


protected:
    // The following functions are actually using OSG code, so they are not compatible with
    // globjects/glbinding include. Therefore, they are separately implemented in OsgPainter_osg.cpp
    void osg_setScene(osg::Node * scene);
    void osg_onInitialize();
    void osg_onPaint();
    void osg_cleanup();


protected:
    osgViewer::Viewer                 * m_viewer;
    osgViewer::GraphicsWindowEmbedded * m_embedded;
    osg::Node                         * m_scene;
    int                                 m_viewportX;
    int                                 m_viewportY;
    int                                 m_viewportWidth;
    int                                 m_viewportHeight;

    // Capabilities
    gloperate::ViewportCapability          * m_viewportCapability;
    gloperate::TargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::InputCapability             * m_inputCapability;
    gloperate::VirtualTimeCapability       * m_virtualTimeCapability;


};


} // namespace gloperate_osg
