
#pragma once


#include <QSurfaceFormat>

#include <gloperate/base/AbstractGLContextFactory.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


class OpenGLWindow;


/**
*  @brief
*    OpenGL context factory
*/
class GLOPERATE_QT_API GLContextFactory : public gloperate::AbstractGLContextFactory
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] window
    *    Surface on which contexts are created (must NOT be null)
    */
    GLContextFactory(OpenGLWindow * window);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual gloperate::AbstractGLContext * createContext(const gloperate::GLContextFormat & format) override;


protected:
    QSurfaceFormat toQSurfaceFormat(const gloperate::GLContextFormat & format) const;


protected:
    OpenGLWindow * m_window; ///< Surface on which contexts are created
};


} // namespace gloperate_qt
