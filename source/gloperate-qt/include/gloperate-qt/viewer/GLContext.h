
#pragma once


#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QOpenGLContext;


namespace gloperate_qt
{


class OpenGLWindow;


/**
*  @brief
*    OpenGL context implementation based on Qt
*/
class GLOPERATE_QT_API GLContext : public gloperate::AbstractGLContext
{
    friend class GLContextFactory;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] window
    *    Qt window that contains the context
    *  @param[in] context
    *    Qt OpenGL context
    */
    GLContext(OpenGLWindow * window, QOpenGLContext * context);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContext();

    /**
    *  @brief
    *    Get Qt OpenGL context
    *
    *  @return
    *    Qt OpenGL context
    */
    QOpenGLContext * qtContext() const;

    // Virtual gloperate::AbstractGLContext functions
    virtual void use() const override;
    virtual void release() const override;


protected:
    OpenGLWindow   * m_window;  ///< Qt window that contains the context
    QOpenGLContext * m_context; ///< Qt OpenGL context


protected:
    void destroyContext();


private:
    static void makeCurrent(QOpenGLContext * context, OpenGLWindow * window);
    static void doneCurrent(QOpenGLContext * context);
    static void initGLBinding();
};


} // namespace gloperate_qt
