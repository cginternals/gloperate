
#pragma once


#include <QQuickView>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QOpenGLContext;

namespace gloperate {
    class ViewerContext;
    class RenderSurface;
    class Stage;
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
    *  @param[in] renderStage
    *    Stage that renders into the surface (can be null)
    */
    QuickView(
        gloperate::ViewerContext * viewerContext
      , gloperate::Stage * renderStage = nullptr
    );

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
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the window (can be null)
    */
    gloperate::Stage * renderStage() const;

    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] stage
    *    Render stage that renders into the window (can be null)
    *
    *  @remarks
    *    When setting a new render stage, the old render stage is destroyed.
    *    The window takes ownership over the stage.
    */
    void setRenderStage(gloperate::Stage * stage);


protected:
    QuickView(gloperate::ViewerContext * viewerContext,
              gloperate::RenderSurface * surface);


protected:
    void onSceneGraphInitialized();
    void onOpenglContextCreated(QOpenGLContext * context);


protected:
    gloperate::ViewerContext * m_viewerContext; ///< Viewer context to which the window belongs (must NOT be null)
    gloperate::RenderSurface * m_surface;       ///< Surface that renders on the window (must NOT be null)
//  QTimer                     m_timer;         ///< Timer for continuous update
};


} // namespace gloperate_qtquick
