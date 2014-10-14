#pragma once


#include <gloperate/input/MouseInputHandler.h>
#include <gloperate-osg/gloperate-osg_api.h>
#include <osg/ref_ptr>


namespace osgViewer {
    class GraphicsWindowEmbedded;
}


namespace gloperate_osg
{


class GLOPERATE_OSG_API OsgMouseHandler : public gloperate::MouseInputHandler
{


public:
    OsgMouseHandler(osgViewer::GraphicsWindowEmbedded * embedded);
    virtual ~OsgMouseHandler();

    virtual void onMouseMove(int x, int y) override;
    virtual void onMousePress(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseWheel(int dx, int dy) override;


protected:
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_embedded; /**< Interface that acts like a window to OSG */


};


} // namespace gloperate_osg
