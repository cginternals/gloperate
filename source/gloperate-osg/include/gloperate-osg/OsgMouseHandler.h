
#pragma once


#include <osg/ref_ptr>

#include <gloperate/input/MouseInputHandler.h>

#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer 
{
    class GraphicsWindowEmbedded;
}


namespace gloperate_osg
{


class OsgRenderStage;


/**
*  @brief
*    Input handler that transfers mouse events to OSG
*/
class GLOPERATE_OSG_API OsgMouseHandler : public gloperate::MouseInputHandler
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] embedded
    *    Interface that acts like a window to OSG
    *  @param[in] stage
    *    Render stage that is informed about changes, can be null
    */
    OsgMouseHandler(osgViewer::GraphicsWindowEmbedded * embedded, OsgRenderStage * stage = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OsgMouseHandler();

    virtual void onMouseMove(int x, int y) override;
    virtual void onMousePress(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button) override;
    virtual void onMouseWheel(int dx, int dy) override;


protected:
    osgViewer::GraphicsWindowEmbedded * m_embedded; /**< Interface that acts like a window to OSG */
    OsgRenderStage                    * m_stage;    /**< Render stage to which the input handler belongs (can be null) */
};


} // namespace gloperate_osg
