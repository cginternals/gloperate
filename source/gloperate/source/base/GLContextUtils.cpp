
#include <gloperate/base/GLContextUtils.h>

#include <cassert>

#include <cppassist/logging/logging.h>

#include <glbinding/gl/gl.h>

#include <glbinding-aux/ContextInfo.h>


using namespace gl;


namespace gloperate
{


gloperate::GLContextFormat GLContextUtils::retrieveFormat()
{
    // Create context format description
    gloperate::GLContextFormat format;

    // Retrieve format
    GLint i;
    GLboolean b;

    format.setVersion(retrieveVersion());

    format.setProfile(retrieveProfile());

    if (format.profile() != GLContextFormat::Profile::Core)
    {
        i = -1; glGetIntegerv(GLenum::GL_RED_BITS, &i);
        format.setRedBufferSize(i);

        i = -1; glGetIntegerv(GLenum::GL_GREEN_BITS, &i);
        format.setGreenBufferSize(i);

        i = -1; glGetIntegerv(GLenum::GL_BLUE_BITS, &i);
        format.setBlueBufferSize(i);

        i = -1; glGetIntegerv(GLenum::GL_ALPHA_BITS, &i);
        format.setAlphaBufferSize(i);

        i = -1; glGetIntegerv(GLenum::GL_DEPTH_BITS, &i);
        format.setDepthBufferSize(i);

        i = -1; glGetIntegerv(GLenum::GL_STENCIL_BITS, &i);
        format.setStencilBufferSize(i);

        b = GL_FALSE; glGetBooleanv(GLenum::GL_STEREO, &b);
        format.setStereo(b == GL_TRUE);
    }

    i = -1; glGetIntegerv(GLenum::GL_SAMPLES, &i);
    format.setSamples(i);

    return format;
}

glbinding::Version GLContextUtils::retrieveVersion()
{
    return glbinding::aux::ContextInfo::version();
}

GLContextFormat::Profile GLContextUtils::retrieveProfile()
{
    gl::ContextProfileMask profileMask = gl::GL_NONE_BIT;
    glGetIntegerv(GLenum::GL_CONTEXT_PROFILE_MASK, reinterpret_cast<GLint*>(&profileMask));

    if (static_cast<GLint>(profileMask) <= 0) // probably a context < 3.2 with no support for profiles
    {
        return GLContextFormat::Profile::None;
    }

    if ((profileMask & GL_CONTEXT_CORE_PROFILE_BIT) != gl::GL_NONE_BIT)
    {
        return GLContextFormat::Profile::Core;
    }

    if ((profileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) != gl::GL_NONE_BIT)
    {
        return GLContextFormat::Profile::Compatibility;
    }

    return GLContextFormat::Profile::None;
}

std::string GLContextUtils::version()
{
    return glbinding::aux::ContextInfo::version().toString();
}

std::string GLContextUtils::profile()
{
    return GLContextFormat::profileString(retrieveProfile());
}

std::string GLContextUtils::vendor()
{
    return glbinding::aux::ContextInfo::vendor();
}

std::string GLContextUtils::renderer()
{
    return glbinding::aux::ContextInfo::renderer();
}


} // namespace gloperate
