
#include <gloperate/base/GLContextUtils.h>

#include <cassert>

#include <globjects/base/baselogging.h>

#include <glbinding/ContextInfo.h>
#include <glbinding/gl/gl.h>


using namespace gl;


namespace gloperate
{


bool GLContextUtils::isValid()
{
    return (tryFetchHandle() > 0);
}

glbinding::ContextHandle GLContextUtils::tryFetchHandle()
{
    const glbinding::ContextHandle handle = glbinding::getCurrentContext();

    if (handle == 0)
    {
        globjects::critical("Acquiring OpenGL context handle failed.");
    }

    return handle;
}

glbinding::Version GLContextUtils::retrieveVersion()
{
    assert(0 != glbinding::getCurrentContext());

    GLint major = -1;
    GLint minor = -1;

    glGetIntegerv(GLenum::GL_MAJOR_VERSION, &major); // major version
    glGetIntegerv(GLenum::GL_MINOR_VERSION, &minor); // minor version

    if (major < 0 && minor < 0) // probably a context < 3.0 with no support for GL_MAJOR/MINOR_VERSION
    {
        const GLubyte * vstr = glGetString(GLenum::GL_VERSION);
        if (!vstr)
        {
            return glbinding::Version();
        }

        assert(vstr[1] == '.');

        assert(vstr[0] >= '0'  && vstr[0] <= '9');
        major = vstr[0] - '0';

        assert(vstr[2] >= '0'  && vstr[2] <= '9');
        minor = vstr[2] - '0';
    }

    return glbinding::Version(major, minor);
}

std::string GLContextUtils::version()
{
    assert(0 != glbinding::getCurrentContext());

    return glbinding::ContextInfo::version().toString();
}

std::string GLContextUtils::vendor()
{
    assert(0 != glbinding::getCurrentContext());

    return glbinding::ContextInfo::vendor();
}

std::string GLContextUtils::renderer()
{
    assert(0 != glbinding::getCurrentContext());

    return glbinding::ContextInfo::renderer();
}


} // namespace gloperate
