
#include <gloperate-glfw/GLContext.h>

#include <cassert>

#ifdef WIN32
    #include <windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..

#include <glbinding/Binding.h>

#include <globjects/base/baselogging.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>


using namespace gl;
using namespace globjects;
using namespace gloperate;


namespace gloperate_glfw
{


glbinding::Version GLContext::maxSupportedVersion()
{
    glbinding::Version version;

    // GLFW3 does not set default hint values on window creation so at least
    // the default values must be set before glfwCreateWindow can be called.
    // cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

    // Create window for version check
    GLFWwindow * window = glfwCreateWindow(1, 1, "VersionCheck", nullptr, nullptr);
    if (window)
    {
        glfwMakeContextCurrent(window);

        glbinding::Binding::initialize(false);
        version = GLContextUtils::retrieveVersion();

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
            version = GLContextUtils::retrieveVersion();

            glfwMakeContextCurrent(nullptr);
            glfwDestroyWindow(window);
        }
    }

    return version;
}

GLFWwindow * GLContext::createWindow(const GLContextFormat & format)
{
    // Check if version is valid and supported
    glbinding::Version version = format.version() < glbinding::Version(3, 0) ? maxSupportedVersion() : GLContextFormat::validateVersion(format.version(), maxSupportedVersion());

    // GLFW3 does not set default hint values on window creation so at least
    // the default values must be set before glfwCreateWindow can be called.
    // cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
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
        glfwWindowHint(GLFW_OPENGL_PROFILE, format.profile() == GLContextFormat::Profile::Core ? GLFW_OPENGL_CORE_PROFILE :
           (format.profile() == GLContextFormat::Profile::Compatibility? GLFW_OPENGL_COMPAT_PROFILE : GLFW_OPENGL_ANY_PROFILE));
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

GLContext::GLContext(GLFWwindow * window)
: m_format(nullptr)
, m_window(window)
{
    assert(window);

    GLFWwindow * current = glfwGetCurrentContext();
    if (current != m_window)
        glfwMakeContextCurrent(m_window);

    m_handle = GLContextUtils::tryFetchHandle();

    if (current != m_window)
        glfwMakeContextCurrent(current);
}

GLContext::~GLContext()
{
    delete m_format;
}

glbinding::ContextHandle GLContext::handle() const
{
    return m_handle;
}

const GLContextFormat & GLContext::format() const
{
    // Return format if already retrieved
    if (m_format)
    {
        return *m_format;
    }

    // Create and retrieve format if not done already
    m_format = new GLContextFormat();

    // Activate context
    GLFWwindow * current = glfwGetCurrentContext();
    if (current != m_window)
    {
        glfwMakeContextCurrent(m_window);
    }

    // Read context format
    assert(isValid());
    *m_format = GLContextUtils::retrieveFormat();

    // Deactivate context
    if (current != m_window)
    {
        glfwMakeContextCurrent(current);
    }

    // Return format description
    return *m_format;
}

void GLContext::use() const
{
    if (m_window)
    {
        glfwMakeContextCurrent(m_window);
    }
}

void GLContext::release() const
{
    if (m_window && m_window == glfwGetCurrentContext())
    {
        glfwMakeContextCurrent(nullptr);
    }
}


} // namespace gloperate_glfw
