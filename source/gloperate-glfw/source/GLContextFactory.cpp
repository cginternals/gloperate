
#include <gloperate-glfw/GLContextFactory.h>

#ifdef WIN32
    #include <windows.h>
#endif

#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..

#include <cppassist/memory/make_unique.h>

#include <glbinding/Binding.h>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-glfw/GLContext.h>


using namespace gloperate;


namespace gloperate_glfw
{


GLContextFactory::GLContextFactory(GLFWmonitor * monitor, unsigned int width, unsigned int height)
: m_monitor(monitor)
, m_width(width)
, m_height(height)
{
}

GLContextFactory::~GLContextFactory()
{
}

std::unique_ptr<gloperate::AbstractGLContext> GLContextFactory::createContext(const gloperate::GLContextFormat & format) const
{
    // Adjust GLFW settings to produce the given context format
    initializeGLFWState(format);

    // Create window
    GLFWwindow * window = glfwCreateWindow(m_width, m_height, "", m_monitor, nullptr);
    if (!window)
    {
        return nullptr;
    }

    // Create context wrapper
    auto context = cppassist::make_unique<GLContext>(window);

    // Handle swap behavior
    context->updateSwapBehavior(format.swapBehavior());

    // Return context (and window)
    return std::move(context);
}

void GLContextFactory::initializeGLFWState(const gloperate::GLContextFormat & format)
{
    // GLFW3 does not set default hint values on window creation so at least
    // the default values must be set before glfwCreateWindow can be called.
    // cf. http://www.glfw.org/docs/latest/group__window.html#ga4fd9e504bb937e79588a0ffdca9f620b
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_VISIBLE, false);

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, format.majorVersion());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, format.minorVersion());
    glfwWindowHint(GLFW_DOUBLEBUFFER, int(true));

    // Set OpenGL context flags
    if (format.version() >= glbinding::Version(3, 0))
    {
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, format.forwardCompatible());
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, format.debugContext());
    }

    // Set OpenGL profile
    switch (format.profile())
    {
        case gloperate::GLContextFormat::Profile::Core:
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            break;

        case gloperate::GLContextFormat::Profile::Compatibility:
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
            break;

        default:
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
            break;
    }

    // Set buffer options
    glfwWindowHint(GLFW_RED_BITS,     format.redBufferSize());
    glfwWindowHint(GLFW_GREEN_BITS,   format.greenBufferSize());
    glfwWindowHint(GLFW_BLUE_BITS,    format.blueBufferSize());
    glfwWindowHint(GLFW_ALPHA_BITS,   format.alphaBufferSize());
    glfwWindowHint(GLFW_DEPTH_BITS,   format.depthBufferSize());
    glfwWindowHint(GLFW_STENCIL_BITS, format.stencilBufferSize());
    glfwWindowHint(GLFW_STEREO,       format.stereo());
    glfwWindowHint(GLFW_SAMPLES,      format.samples());
}


} // namespace gloperate_glfw
