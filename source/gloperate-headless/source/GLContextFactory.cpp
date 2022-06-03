
#include <gloperate-headless/GLContextFactory.h>

#include <cppassist/memory/make_unique.h>

#include <eglbinding/egl/egl.h>

#include <glbinding/Binding.h>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-headless/GLContext.h>


using namespace gloperate;
using namespace egl;


namespace
{


EGLint profileToEGLBits(GLContextFormat::Profile profile)
{
    switch (profile)
    {
    case GLContextFormat::Profile::Core:
        return static_cast<EGLint>(EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT);
    case GLContextFormat::Profile::Compatibility:
        return static_cast<EGLint>(EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT);
    default:
        return 0;
    }
}


} // namespace


namespace gloperate_headless
{


GLContextFactory::GLContextFactory(EGLDisplay display, unsigned int width, unsigned int height)
: m_display(display)
, m_width(width)
, m_height(height)
{
}

GLContextFactory::~GLContextFactory()
{
}

std::unique_ptr<gloperate::AbstractGLContext> GLContextFactory::createContext(const gloperate::GLContextFormat & format) const
{
    const EGLint configAttribs[] = {
        static_cast<EGLint>(EGL_SURFACE_TYPE), static_cast<EGLint>(EGL_PBUFFER_BIT),
        static_cast<EGLint>(EGL_BLUE_SIZE), 8,
        static_cast<EGLint>(EGL_GREEN_SIZE), 8,
        static_cast<EGLint>(EGL_RED_SIZE), 8,
        static_cast<EGLint>(EGL_RENDERABLE_TYPE), static_cast<EGLint>(EGL_OPENGL_BIT),
        static_cast<EGLint>(EGL_NONE)
    };

    const EGLint pbufferAttribs[] = {
        static_cast<EGLint>(EGL_WIDTH), static_cast<EGLint>(m_width),
        static_cast<EGLint>(EGL_HEIGHT), static_cast<EGLint>(m_height),
        static_cast<EGLint>(EGL_NONE)
    };

    EGLint numConfigs;
    std::array<EGLConfig, 128> eglCfgs;

    eglChooseConfig(m_display, configAttribs, eglCfgs.data(), eglCfgs.size(), &numConfigs);

    // Create a surface
    EGLSurface eglSurf = eglCreatePbufferSurface(m_display, eglCfgs[0], pbufferAttribs);

    if (eglSurf == nullptr)
    {
        return nullptr;
    }

    eglBindAPI(EGL_OPENGL_API);

    std::vector<EGLint> ctxattr;

    ctxattr.emplace_back(static_cast<EGLint>(EGL_CONTEXT_MAJOR_VERSION));
    ctxattr.emplace_back(static_cast<EGLint>(format.majorVersion()));
    ctxattr.emplace_back(static_cast<EGLint>(EGL_CONTEXT_MINOR_VERSION));
    ctxattr.emplace_back(static_cast<EGLint>(format.minorVersion()));

    if (format.profile() != GLContextFormat::Profile::None)
    {
        ctxattr.emplace_back(static_cast<EGLint>(EGL_CONTEXT_OPENGL_PROFILE_MASK));
        ctxattr.emplace_back(static_cast<EGLint>(profileToEGLBits(format.profile())));
    }

    /*
    ctxattr.emplace_back(static_cast<EGLint>(EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE));
    ctxattr.emplace_back(static_cast<EGLint>(format.forwardCompatible()));

    ctxattr.emplace_back(static_cast<EGLint>(EGL_CONTEXT_OPENGL_DEBUG));
    ctxattr.emplace_back(static_cast<EGLint>(format.debugContext()));
    */

    ctxattr.emplace_back(static_cast<EGLint>(EGL_NONE));

    auto openGLContext = eglCreateContext(m_display, eglCfgs[0], 0, ctxattr.data());

    if (openGLContext == nullptr)
    {
        return nullptr;
    }

    // Create context wrapper
    auto context = cppassist::make_unique<GLContext>(m_display, eglSurf, openGLContext);

    // Handle swap behavior
    context->updateSwapBehavior(format.swapBehavior());

    // Return surface (and context)
    return context;
}


} // namespace gloperate_headless
