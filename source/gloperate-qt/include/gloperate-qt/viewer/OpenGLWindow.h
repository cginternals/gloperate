
#pragma once


#include <QWindow>
#include <QScopedPointer>
#include <QSurfaceFormat>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate
{
    class GLContextFormat;
}


namespace gloperate_qt
{


class Context;


/**
*  @brief
*    Qt window that initializes an OpenGL context for rendering
*
*  @remarks
*    This base class is for general use of OpenGL and does not depend on
*    using gloperate for rendering. When using gloperate, the derived
*    class RenderWindow should be used.
*/
class GLOPERATE_QT_API OpenGLWindow : public QWindow
{
public:
    /**
    *  @brief
    *    Constructor
    */
    OpenGLWindow();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OpenGLWindow();

    /**
    *  @brief
    *    Set desired context format for the window
    *
    *  @param[in] format
    *    Context format
    *
    *  @remarks
    *    Use this function to set the context format from a
    *    gloperate format description.
    */
    void setContextFormat(const gloperate::GLContextFormat & format);

    /**
    *  @brief
    *    Set desired context format for the window
    *
    *  @param[in] format
    *    Surface format
    *
    *  @remarks
    *    Use this function to set the context format from a
    *    Qt surface format description.
    */
    void setContextFormat(const QSurfaceFormat & format);

    void doIt();

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context
    */
    Context * context() const;

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
    QSurfaceFormat                   m_format;        ///< Desired OpenGL format
    QScopedPointer<QOpenGLContext>   m_qContext;      ///< OpenGL context created and used by the window
    Context                        * m_context;       ///< Context wrapper for gloperate
    bool                             m_initialized;   ///< Has the rendering already been initialized?
    bool                             m_updatePending; ///< Flag to indicate if a redraw has been requested
};


} // namespace gloperate-qt
