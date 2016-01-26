
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QWindow>
#include <QScopedPointer>
#include <QSurfaceFormat>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QOpenGLContext;


namespace gloperate_qt
{


/**
*  @brief
*    Qt window that initializes an OpenGL context for rendering
*
*  @remarks
*    This base class is for general use of OpenGL and does not depend on
*    using gloperate for rendering. When using gloperate, the derived
*    class QtOpenGLWindow should be used.
*/
class GLOPERATE_QT_API QtOpenGLWindowBase : public QWindow
{
public:
    /**
    *  @brief
    *    Get default OpenGL format
    *
    *  @return
    *    Surface format
    */
    static QSurfaceFormat defaultFormat();


public:
    /**
    *  @brief
    *    Constructor
    */
    QtOpenGLWindowBase();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] format
    *    Surface format
    */
    QtOpenGLWindowBase(const QSurfaceFormat & format);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtOpenGLWindowBase();

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context
    */
    QOpenGLContext * context() const;

    /**
    *  @brief
    *    Request update (repaint) of the window
    */
    void updateGL();

    /**
    *  @brief
    *    Initialize OpenGL rendering
    */
    void initialize();

    /**
    *  @brief
    *    Make associated OpenGL context current
    */
	void makeCurrent();

    /**
    *  @brief
    *    Release associated OpenGL context
    */
	void doneCurrent();


protected:
    /**
    *  @brief
    *    Resize OpenGL scene
    */
    void resize(QResizeEvent * event);

    /**
    *  @brief
    *    Render OpenGL scene
    */
    void paint();


protected:
    /**
    *  @brief
    *    Called once after OpenGL has been initialized
    */
    virtual void onInitialize();

    /**
    *  @brief
    *    Called whenever the window has been resized
    */
    virtual void onResize(QResizeEvent * event);

    /**
    *  @brief
    *    Called to redraw the OpenGL scene
    */
    virtual void onPaint();

    // Qt event callbacks
    virtual bool event(QEvent * event) override;
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void exposeEvent(QExposeEvent * event) override;
    virtual void enterEvent(QEvent * event);
    virtual void leaveEvent(QEvent * event);


protected:
    QScopedPointer<QOpenGLContext> m_context;       ///< OpenGL context created and used by the window
    bool                           m_initialized;   ///< Has the rendering already been initialized?
    bool                           m_updatePending; ///< Flag to indicate if a redraw has been requested
};


} // namespace gloperate-qt
