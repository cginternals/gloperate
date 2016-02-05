
#pragma once


#include <gloperate-qt/viewer/OpenGLWindow.h>


namespace gloperate {
    class ViewerContext;
    class Surface;
}


namespace gloperate_qt
{


/**
*  @brief
*    Window that renders a gloperate scene
*/
class GLOPERATE_QT_API RenderWindow : public OpenGLWindow
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    */
    RenderWindow(gloperate::ViewerContext * viewerContext);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    *  @param[in] format
    *    Surface format
    */
    RenderWindow(gloperate::ViewerContext * viewerContext, const QSurfaceFormat & format);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderWindow();
};


} // namespace gloperate-qt
