#pragma once


// Do not include any osg headers OR gloperate headers that pull in globjects/glbinding,
// because that would break the separation between glbinding and gl.h.
// Use only pointers to OSG objects, call ref/unref to maintain memory management,
// and separate all calls to osg in osg_ functions implemented inside OsgRenderStage_osg.cpp.


#include <gloperate/pipelines/AbstractStage.h>
#include <gloperate/pipelines/InputSlot.h>
#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer {
    class Viewer;
    class GraphicsWindowEmbedded;
}
namespace osg {
    class Node;
}
namespace gloperate {
    class AbstractViewportCapability;
}


namespace gloperate_osg
{


class OsgKeyboardHandler;
class OsgMouseHandler;


class GLOPERATE_OSG_API OsgRenderStage : public gloperate::AbstractStage
{


public:
    OsgRenderStage(const std::string & name);
    virtual ~OsgRenderStage();

    osgViewer::Viewer * viewer() const;
    osg::Node * scene() const;
    void setScene(osg::Node * scene);
    OsgKeyboardHandler * createKeyboardHandler() const;
    OsgMouseHandler * createMouseHandler() const;


protected:
    virtual void initialize() override;
    virtual void process() override;
    virtual void handleViewportChanged();
    virtual void postOsgRendering();


protected:
    // The following functions are actually using OSG code, so they are not compatible with
    // globjects/glbinding include. Therefore, they are separately implemented in OsgPainter_osg.cpp
    void osg_setScene(osg::Node * scene);
    void osg_initialize();
    void osg_process();
    void osg_cleanup();


public:
    // Input data
    gloperate::InputSlot<gloperate::AbstractViewportCapability *>  m_viewport;


protected:
    // OSG scene data
    osgViewer::Viewer                 * m_viewer;
    osgViewer::GraphicsWindowEmbedded * m_embedded;
    osg::Node                         * m_scene;
    int                                 m_viewportX;
    int                                 m_viewportY;
    int                                 m_viewportW;
    int                                 m_viewportH;


};


} // namespace gloperate_osg
