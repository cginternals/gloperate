#pragma once


#include <gloperate/input/KeyboardInputHandler.h>
#include <gloperate-osg/gloperate-osg_api.h>
#include <osg/ref_ptr>


namespace osgViewer {
    class GraphicsWindowEmbedded;
}


namespace gloperate_osg
{


class GLOPERATE_OSG_API OsgKeyboardHandler : public gloperate::KeyboardInputHandler
{


public:
    OsgKeyboardHandler(osgViewer::GraphicsWindowEmbedded * embedded);
    virtual ~OsgKeyboardHandler();

    virtual void onKeyDown(gloperate::Key key) override;
    virtual void onKeyUp(gloperate::Key key) override;


protected:
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_embedded;


};


} // namespace gloperate_osg
