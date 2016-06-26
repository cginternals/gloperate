
#include <gloperate/base/GLContextFormat.h>

#include <cassert>
#include <sstream>
#include <map>

#include <cppassist/logging/logging.h>


namespace gloperate
{


const std::string & GLContextFormat::profileString(const Profile profile)
{
    static const std::map<Profile, std::string> profileIdentifier =
    {
        { Profile::Core,          "Core" }
      , { Profile::Compatibility, "Compatibility" } 
      , { Profile::None,          "None" }
    };

    return profileIdentifier.at(profile);
}

const std::string & GLContextFormat::swapBehaviorString(const SwapBehavior swapBehavior)
{
    static const std::map<SwapBehavior, std::string> swapbIdentifier =
    {
        { SwapBehavior::Default,         "Default" }
      , { SwapBehavior::DoubleBuffering, "DoubleBuffering" }
      , { SwapBehavior::SingleBuffering, "SingleBuffering" } 
      , { SwapBehavior::TripleBuffering, "TripleBuffering" }
    };

    return swapbIdentifier.at(swapBehavior);
}

glbinding::Version GLContextFormat::validateVersion(
    const glbinding::Version & requested
  , const glbinding::Version & maximum)
{
    // Determine maximum version
    glbinding::Version maximumVersion = maximum;
    if (maximumVersion.isNull())
    {
        // [TODO] Why these?
#ifdef __APPLE__
        maximumVersion = glbinding::Version(3, 2);
#else
        maximumVersion = glbinding::Version(3, 0);
#endif
    }

    // Use maximum version if specified version is not suitable
    if (requested.isNull() || requested > maximumVersion)
    {
        return maximumVersion;
    }

    // If specified version in invalid, try to find the nearest version
    else if (!requested.isValid())
    {
        glbinding::Version nearest = requested.nearest();
        return nearest > maximumVersion ? maximumVersion : nearest;
    }

    // Otherwise, use specified version
    else
    {
        return requested;
    }
}

GLContextFormat::GLContextFormat()
: m_version(glbinding::Version(4, 5))
, m_profile(Profile::None)
, m_forwardCompatibility(false)
, m_debugContext(false)
, m_redBufferSize(-1)
, m_greenBufferSize(-1)
, m_blueBufferSize(-1)
, m_alphaBufferSize(-1)
, m_depthBufferSize(24)
, m_stencilBufferSize(0)
, m_stereo(false)
, m_samples(-1)
, m_swapBehavior(SwapBehavior::DoubleBuffering)
{
#ifdef __APPLE__
    m_version = glbinding::Version(3,2);
    m_profile = Profile::Core;
#endif
}

GLContextFormat::~GLContextFormat()
{
}

const glbinding::Version & GLContextFormat::version() const
{
    return m_version;
}

int GLContextFormat::majorVersion() const
{
    return m_version.majorVersion();
}

int GLContextFormat::minorVersion() const
{
    return m_version.minorVersion();
}

void GLContextFormat::setVersion(const glbinding::Version & version)
{
    m_version = version;
}

void GLContextFormat::setVersion(
    const unsigned int majorVersion
  , const unsigned int minorVersion)
{
    setVersion(glbinding::Version(majorVersion, minorVersion));
}

GLContextFormat::Profile GLContextFormat::profile() const
{
    return m_profile;
}

void GLContextFormat::setProfile(const GLContextFormat::Profile profile)
{
    m_profile = profile;
}

bool GLContextFormat::forwardCompatible() const
{
    return m_forwardCompatibility;
}

void GLContextFormat::setForwardCompatible(const bool on)
{
    m_forwardCompatibility = on;
}

bool GLContextFormat::debugContext() const
{
    return m_debugContext;
}

void GLContextFormat::setDebugContext(const bool on)
{
    m_debugContext = on;
}

int GLContextFormat::redBufferSize() const
{
    return m_redBufferSize;
}

void GLContextFormat::setRedBufferSize(const int size)
{
    m_redBufferSize = size;
}

int GLContextFormat::greenBufferSize() const
{
    return m_greenBufferSize;
}

void GLContextFormat::setGreenBufferSize(const int size)
{
    m_greenBufferSize = size;
}

int  GLContextFormat::blueBufferSize() const
{
    return m_blueBufferSize;
}

void GLContextFormat::setBlueBufferSize(const int size)
{
    m_blueBufferSize = size;
}

int GLContextFormat::alphaBufferSize() const
{
    return m_alphaBufferSize;
}

void GLContextFormat::setAlphaBufferSize(const int size)
{
    m_alphaBufferSize = size;
}

int GLContextFormat::depthBufferSize() const
{
    return m_depthBufferSize;
}

void GLContextFormat::setDepthBufferSize(const int size)
{
    m_depthBufferSize = size;
}

int GLContextFormat::stencilBufferSize() const
{
    return m_stencilBufferSize;
}

void GLContextFormat::setStencilBufferSize(const int size)
{
    m_stencilBufferSize = size;
}

bool GLContextFormat::stereo() const
{
    return m_stereo;
}

void GLContextFormat::setStereo(const bool on)
{
    m_stereo = on;
}

int GLContextFormat::samples() const
{
    return m_samples;
}

void GLContextFormat::setSamples(const int samples)
{
    m_samples = samples;
}

GLContextFormat::SwapBehavior GLContextFormat::swapBehavior() const
{
    return m_swapBehavior;
}

void GLContextFormat::setSwapBehavior(const GLContextFormat::SwapBehavior behavior)
{
    m_swapBehavior = behavior;
}

bool GLContextFormat::verify(const GLContextFormat & requested) const
{
   return verifyVersionAndProfile(requested)
             && verifyPixelFormat(requested);
}

bool GLContextFormat::verifyVersionAndProfile(const GLContextFormat & requested) const
{
    bool sameProfiles = (requested.profile() == profile());

    if (!sameProfiles)
    {
        cppassist::warning() << "Profile mismatch for the current context: "
            << profileString(requested.profile()) << " requested, "
            << profileString(profile())           << " created.";
    }

    if (requested.version() != version())
    {
        cppassist::warning() << "Version mismatch for the current context: "
            << requested.version().toString() << " requested, "
            << version().toString()           << " created.";

        if (requested.profile() == Profile::Core)
        {
            return false;
        }
    }

    return sameProfiles;
}

bool GLContextFormat::verifyPixelFormat(const GLContextFormat & requested) const
{
    std::vector<std::string> issues;

    bool sameSwapBehaviors = (requested.swapBehavior() == swapBehavior());

    if (!sameSwapBehaviors)
    {
        cppassist::warning() << "Swap behavior mismatch for the current context: "
            << swapBehaviorString(requested.swapBehavior()) << " requested, "
            << swapBehaviorString(swapBehavior())           << " created.";
    }

    if (requested.depthBufferSize())
    {
        if (!depthBufferSize())
        {
            issues.push_back("- Depth Buffer requested, but none created.");
        }
        else
        {
            verifyBufferSize(requested.depthBufferSize(), depthBufferSize(),
                "- Depth Buffer", issues
            );
        }
    }

    verifyBufferSize(requested.redBufferSize(),   redBufferSize(),
        "- Red Buffer", issues);
    verifyBufferSize(requested.greenBufferSize(), greenBufferSize(),
        "- Green Buffer", issues);
    verifyBufferSize(requested.blueBufferSize(),  blueBufferSize(),
        "- Blue Buffer", issues);
    verifyBufferSize(requested.alphaBufferSize(), alphaBufferSize(),
        "- Alpha Buffer", issues);

    if (requested.stencilBufferSize())
    {
        if (!stencilBufferSize())
        {
            issues.push_back("- Stencil Buffer requested, but none created.");
        }
        else
        {
            verifyBufferSize(requested.stencilBufferSize(), stencilBufferSize(),
                "- Stencil Buffer", issues);
        }
    }

    if (requested.stereo() && !stereo())
    {
        issues.push_back("- Stereo Buffering requested, but not initialized.");
    }

    if (requested.samples())
    {
        if (!samples())
        {
            issues.push_back("- Sample Buffers requested, but none initialized.");
        }
        else
        {
            verifyBufferSize(requested.samples(), samples(), "- Samples ", issues);
        }
    }

    if (issues.empty())
    {
        return true;
    }

    cppassist::warning() << "Pixelformat mismatch for the current context:";
    for (const std::string & issue : issues)
    {
        cppassist::warning() << issue;
    }

    return false;
}

void GLContextFormat::verifyBufferSize(
    const unsigned int sizeRequested
  , const unsigned int sizeInitialized
  , const std::string & warning
  , std::vector<std::string> & issues) const
{
    if (sizeRequested == sizeInitialized)
    {
        return;
    }

    std::stringstream ss;
    ss << warning << " size mismatch: " << sizeRequested << " requested, " << sizeInitialized << " created.";
    issues.push_back(ss.str());
}


} // namespace gloperate
