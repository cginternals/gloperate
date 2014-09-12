
#include <gloperate-glfw/Context.h>

#include <cassert>

#include <glbinding/Version.h>
#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>

#include <gloperate/ContextFormat.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..

using namespace gl;
using namespace globjects;
using namespace gloperate;

namespace gloperate_glfw
{

glbinding::Version Context::maxSupportedVersion()
{
    glbinding::Version version;

    /*
    * GLFW3 does not set default hint values on window creation so at least
    * the default values must be set before glfwCreateWindow can be called.
    * cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    */
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

#ifdef MAC_OS
    /*
    * Using OS X the following hints must be set for proper context initialization
    * (cf. http://stackoverflow.com/questions/19969937/getting-a-glsl-330-context-on-osx-10-9-mavericks)
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

    // create window for version check
    GLFWwindow * window = glfwCreateWindow(1, 1, "VersionCheck", nullptr, nullptr);

    if (window)
    {
        glfwMakeContextCurrent(window);

        GLint major, minor;
        glGetIntegerv(GLenum::GL_MAJOR_VERSION, &major); // major version
        glGetIntegerv(GLenum::GL_MINOR_VERSION, &minor); // minor version

        version = glbinding::Version(major, minor);

        glfwMakeContextCurrent(nullptr);
        glfwDestroyWindow(window);
    }
    return version;
}

GLFWwindow * Context::create(const ContextFormat & format)
{
    // check if version is valid and supported
    glbinding::Version version = ContextFormat::validateVersion(format.version(), maxSupportedVersion());

    /*
    * GLFW3 does not set default hint values on window creation so at least
    * the default values must be set before glfwCreateWindow can be called.
    * cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    */
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.m_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.m_minor);

#ifdef MAC_OS

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#else

    if (version >= glbinding::Version(3, 0))
    {
        if (format.forwardCompatible())
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
        if (format.debugContext())
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }

    if (version >= glbinding::Version(3, 2))
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, format.profile() == ContextFormat::Profile::Core ? GLFW_OPENGL_CORE_PROFILE :
           (format.profile() == ContextFormat::Profile::Compatibility? GLFW_OPENGL_COMPAT_PROFILE : GLFW_OPENGL_ANY_PROFILE));
    }

#endif

    glfwWindowHint(GLFW_DEPTH_BITS,   format.depthBufferSize());
    glfwWindowHint(GLFW_STENCIL_BITS, format.stencilBufferSize());

    glfwWindowHint(GLFW_RED_BITS,     format.redBufferSize());
    glfwWindowHint(GLFW_GREEN_BITS,   format.greenBufferSize());
    glfwWindowHint(GLFW_BLUE_BITS,    format.blueBufferSize());
    glfwWindowHint(GLFW_ALPHA_BITS,   format.alphaBufferSize());

    glfwWindowHint(GLFW_STEREO, format.stereo());
    glfwWindowHint(GLFW_SAMPLES, format.samples());

    GLFWwindow * window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

    if (window)
    {
        glfwMakeContextCurrent(window);
        glfwSwapInterval(static_cast<int>(format.swapBehavior()));
        glfwMakeContextCurrent(0);
    }
    return window;
}


Context::Context(GLFWwindow * window)
: m_window(window)
, m_format(nullptr)
{
    if (window)
    {
        GLFWwindow * current = glfwGetCurrentContext();
        if (current != m_window)
            glfwMakeContextCurrent(m_window);

        m_handle = tryFetchHandle();

        if (current != m_window)
            glfwMakeContextCurrent(current);
    }
}

Context::~Context()
{
    delete m_format;
}

glbinding::ContextHandle Context::handle() const
{
    return m_handle;
}

const ContextFormat & Context::format() const
{
    assert(isValid());

    if (m_format)
        return *m_format;

    // create and retrive format if not done already

    m_format = new ContextFormat();

    GLFWwindow * current = glfwGetCurrentContext();
    if (current != m_window)
        glfwMakeContextCurrent(m_window);

    GLint i;
    GLboolean b;

    glGetIntegerv(GLenum::GL_DEPTH_BITS, &i);
    m_format->setDepthBufferSize(i);
    glGetIntegerv(GLenum::GL_STENCIL_BITS, &i);
    m_format->setStencilBufferSize(i);

    glGetIntegerv(GLenum::GL_RED_BITS, &i);
    m_format->setRedBufferSize(i);
    glGetIntegerv(GLenum::GL_GREEN_BITS, &i);
    m_format->setGreenBufferSize(i);
    glGetIntegerv(GLenum::GL_BLUE_BITS, &i);
    m_format->setBlueBufferSize(i);
    glGetIntegerv(GLenum::GL_ALPHA_BITS, &i);
    m_format->setAlphaBufferSize(i);

    glGetIntegerv(GLenum::GL_SAMPLES, &i);
    m_format->setSamples(i);

    glGetBooleanv(GLenum::GL_STEREO, &b);
    m_format->setStereo(static_cast<bool>(b));

    if (current != m_window)
        glfwMakeContextCurrent(current);

    return *m_format;
}

const ContextFormat & Context::format() const
{
	return *m_format;
}

} // namespace gloperate_glfw
