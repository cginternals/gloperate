
#pragma once


#include <QQuickView>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace gloperate {
    class ViewerContext;
}

namespace gloperate_qt {
    class GLContext;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick window with support for gloperate
*/
class GLOPERATE_QTQUICK_API QuickView : public QQuickView
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the window belongs (must NOT be null!)
    */
    QuickView(gloperate::ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QuickView();

    /**
    *  @brief
    *    Get viewer context
    *
    *  @return
    *    Viewer context (cannot be null)
    */
    gloperate::ViewerContext * viewerContext() const;

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context (can be null)
    */
    gloperate_qt::GLContext * context() const;


protected:
    void onSceneGraphInitialized();
    void onBeforeRendering();


protected:
    gloperate::ViewerContext * m_viewerContext; ///< Viewer context to which the window belongs (must NOT be null)
    gloperate_qt::GLContext  * m_context;       ///< Context wrapper for gloperate (can be null)
};


} // namespace gloperate_qtquick
