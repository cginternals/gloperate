
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


class QmlEngine;


/**
*  @brief
*    Qt quick window with support for gloperate rendering
*/
class GLOPERATE_QTQUICK_API QuickView : public QQuickView
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] engine
    *    Qml engine with gloperate integration (must NOT be null!)
    *  @param[in] parent
    *    Parent window (can be null)
    */
    QuickView(QmlEngine * engine, QWindow * parent = nullptr);

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
