
#include <gloperate-glfw/GLContextFactory.h>

#ifdef WIN32
    #include <windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..

#include <glbinding/Binding.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>

#include <gloperate-glfw/GLContext.h>


using namespace gloperate;


namespace gloperate_glfw
{


GLContextFactory::GLContextFactory()
{
}

GLContextFactory::~GLContextFactory()
{
}

gloperate::AbstractGLContext * GLContextFactory::createContext(const gloperate::GLContextFormat & format)
{
    // Check if version is valid and supported
    glbinding::Version version =
        (format.version() < glbinding::Version(3, 0)) ?
            maxSupportedVersion() :
            GLContextFormat::validateVersion(format.version(), maxSupportedVersion());

    // GLFW3 does not set default hint values on window creation so at least
    // the default values must be set before glfwCreateWindow can be called.
    // cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.majorVersion());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minorVersion());

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

    glfwWindowHint(GLFW_RED_BITS,     format.redBufferSize());
    glfwWindowHint(GLFW_GREEN_BITS,   format.greenBufferSize());
    glfwWindowHint(GLFW_BLUE_BITS,    format.blueBufferSize());
    glfwWindowHint(GLFW_ALPHA_BITS,   format.alphaBufferSize());
    glfwWindowHint(GLFW_DEPTH_BITS,   format.depthBufferSize());
    glfwWindowHint(GLFW_STENCIL_BITS, format.stencilBufferSize());
    glfwWindowHint(GLFW_STEREO,       format.stereo());
    glfwWindowHint(GLFW_SAMPLES,      format.samples());

    // Create window
    GLFWwindow * window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (!window)
    {
        return nullptr;
    }

    // Activate context
    glfwMakeContextCurrent(window);
    glfwSwapInterval(static_cast<int>(format.swapBehavior()));

    // Initialize glbinding in that context
    glbinding::Binding::initialize(false);

    // Create context wrapper
    GLContext * context = new GLContext(window);

    // Deactivate context
    glfwMakeContextCurrent(nullptr);

    return context;
}

glbinding::Version GLContextFactory::maxSupportedVersion()
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


} // namespace gloperate_glfw
