
#pragma once


#include <gloperate/base/AbstractGLContextFactory.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


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
    */
    GLContextFactory();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual gloperate::AbstractGLContext * createContext(const gloperate::GLContextFormat & format) override;
};


} // namespace gloperate_qt
