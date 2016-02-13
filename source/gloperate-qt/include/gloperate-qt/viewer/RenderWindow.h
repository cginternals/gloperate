
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
    *    Viewer context to which the window belongs (must NOT be null!)
    */
    RenderWindow(gloperate::ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderWindow();

    /**
    *  @brief
    *    Get surface that is rendered into the window
    *
    *  @return
    *    Surface (cannot be null)
    */
    gloperate::Surface * surface() const;


protected:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] surface
    *    Surface (must NOT be null!)
    */
    RenderWindow(gloperate::Surface * surface);


protected:
    gloperate::Surface * m_surface; ///< Surface that renders on the window (must NOT be null)
};


} // namespace gloperate-qt
