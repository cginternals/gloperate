
#pragma once


#include <gloperate-qt/viewer/OpenGLWindow.h>


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
    */
    RenderWindow();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] format
    *    Surface format
    */
    RenderWindow(const QSurfaceFormat & format);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderWindow();
};


} // namespace gloperate-qt
