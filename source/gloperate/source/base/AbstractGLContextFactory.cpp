
#include <gloperate/base/AbstractGLContextFactory.h>

#include <cppassist/logging/logging.h>


namespace gloperate
{


AbstractGLContextFactory::AbstractGLContextFactory()
{
}

AbstractGLContextFactory::~AbstractGLContextFactory()
{
}

std::unique_ptr<gloperate::AbstractGLContext> AbstractGLContextFactory::createBestContext(const gloperate::GLContextFormat & f) const
{
    gloperate::GLContextFormat format(f);

#ifdef SYSTEM_DARWIN
    if (format.version().majorVersion() != 0 && format.version() != glbinding::Version(3, 2))
    {
        cppassist::info("gloperate") << "Override OpenGL format to 3.2 Core Forward Compatible on macOS";
    }

    // Assume best context for macOS
    // Through the forward compatible flag we always get the highest (and only available) post 3.0 context
    format.setVersion(3, 2);
    format.setProfile(gloperate::GLContextFormat::Profile::Core);
    format.setForwardCompatible(true);
#else
    if (format.majorVersion() < 3)
    {
        // Detect highest possible context version
        format.setVersion(3, 2);
        format.setForwardCompatible(true);
    }
#endif

    auto context = createContext(format);

    if (context)
    {
        return context;
    }

    cppassist::warning("gloperate") << "User-configured context creation failed";

    // Context creation failed, use system default
    gloperate::GLContextFormat safeFormat;
    safeFormat.setVersion(3, 2);
    safeFormat.setProfile(gloperate::GLContextFormat::Profile::Core);
    safeFormat.setForwardCompatible(true);
    safeFormat.setDebugContext(format.debugContext());

    auto safeContext = createContext(safeFormat);

    if (!safeContext)
    {
        cppassist::error("gloperate") << "Safe context creation failed";

        return nullptr;
    }

    return safeContext;
}


} // namespace gloperate
