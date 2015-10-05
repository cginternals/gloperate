
#include <gloperate-glfw/Context.h>

#include <cassert>

#ifdef WIN32
    #include <windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..

#include <glbinding/Binding.h>
#include <glbinding/Version.h>
#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>

#include <gloperate/painter/ContextFormat.h>


using namespace gl;
using namespace globjects;
using namespace gloperate;


namespace gloperate_glfw
{


glbinding::Version Context::maxSupportedVersion()
{
    glbinding::Version version;

    /* GLFW3 does not set default hint values on window creation so at least
        the default values must be set before glfwCreateWindow can be called.
        cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    */
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

    // create window for version check
    GLFWwindow * window = glfwCreateWindow(1, 1, "VersionCheck", nullptr, nullptr);

    if (window)
    {
        glfwMakeContextCurrent(window);

        glbinding::Binding::initialize(false);
        version = retrieveVersion();

        glfwMakeContextCurrent(nullptr);
        glfwDestroyWindow(window);
    }

    if (version <= glbinding::Version(3, 0))
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

        window = glfwCreateWindow(1, 1, "VersionCheck", nullptr, nullptr);

        if (window)
        {
            glfwMakeContextCurrent(window);

            glbinding::Binding::initialize(false);
            version = retrieveVersion();

            glfwMakeContextCurrent(nullptr);
            glfwDestroyWindow(window);
        }
    }

    return version;
}

GLFWwindow * Context::create(const ContextFormat & format)
{
    // check if version is valid and supported
    glbinding::Version version = format.version() < glbinding::Version(3, 0) ? maxSupportedVersion() : ContextFormat::validateVersion(format.version(), maxSupportedVersion());

    /*
    * GLFW3 does not set default hint values on window creation so at least
    * the default values must be set before glfwCreateWindow can be called.
    * cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    */
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.m_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.m_minor);

#ifdef __APPLE__

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#else

    if (version >= glbinding::Version(3, 0))
    {
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, format.forwardCompatible());
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, format.debugContext());
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
        glbinding::Binding::initialize(false);
        glfwSwapInterval(static_cast<int>(format.swapBehavior()));
        glfwMakeContextCurrent(nullptr);
    }
    return window;
}

Context::Context(GLFWwindow * window)
: m_format(nullptr)
, m_window(window)
{
    assert(window);

    GLFWwindow * current = glfwGetCurrentContext();
    if (current != m_window)
        glfwMakeContextCurrent(m_window);

    m_handle = tryFetchHandle();

    if (current != m_window)
        glfwMakeContextCurrent(current);
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

    m_format->setVersion(retrieveVersion());

    GLint i;
    GLboolean b;

    i = -1; glGetIntegerv(GLenum::GL_DEPTH_BITS, &i);
    m_format->setDepthBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_STENCIL_BITS, &i);
    m_format->setStencilBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_RED_BITS, &i);

    m_format->setRedBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_GREEN_BITS, &i);
    m_format->setGreenBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_BLUE_BITS, &i);
    m_format->setBlueBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_ALPHA_BITS, &i);
    m_format->setAlphaBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_SAMPLES, &i);
    m_format->setSamples(i);
    b = GL_FALSE;  glGetBooleanv(GLenum::GL_STEREO, &b);
    m_format->setStereo(b == GL_TRUE);

    if (current != m_window)
        glfwMakeContextCurrent(current);

    return *m_format;
}

void Context::makeCurrent() const
{
    if (m_window)
        glfwMakeContextCurrent(m_window);
}

void Context::doneCurrent() const
{
    if (m_window && m_window == glfwGetCurrentContext())
        glfwMakeContextCurrent(nullptr);
}


} // namespace gloperate_glfw
