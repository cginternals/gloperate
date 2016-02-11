
#include <gloperate/base/GLContextFormat.h>

#include <cassert>
#include <sstream>
#include <map>

#include <globjects/base/baselogging.h>


using namespace globjects;


namespace gloperate
{


GLContextFormat::GLContextFormat()
: m_version(glbinding::Version(4, 5))
, m_profile(Profile::None)
, m_debugContext(false)
, m_forwardCompatibility(false)
, m_redBufferSize(8)
, m_greenBufferSize(8)
, m_blueBufferSize(8)
, m_alphaBufferSize(8)
, m_depthBufferSize(24)
, m_stencilBufferSize(0)
, m_stereo(false)
, m_swapBehavior(SwapBehavior::DoubleBuffering)
, m_samples(0)
{
}

GLContextFormat::~GLContextFormat()
{
}

void GLContextFormat::setVersion(
    const unsigned int majorVersion
,   const unsigned int minorVersion)
{
    setVersion(glbinding::Version(majorVersion, minorVersion));
}

void GLContextFormat::setVersion(const glbinding::Version & version)
{
    m_version = version;
}

glbinding::Version GLContextFormat::validateVersion(const glbinding::Version &requestedVersion
,   const glbinding::Version &_maximumVersion)
{
    glbinding::Version maximumVersion = _maximumVersion;
    if (maximumVersion.isNull())
    {
#ifdef __APPLE__
        maximumVersion = glbinding::Version(3, 2);
#else
        maximumVersion = glbinding::Version(3, 0);
#endif
    }

    if (requestedVersion.isNull() || requestedVersion > maximumVersion)
        return maximumVersion;

    if (!requestedVersion.isValid())
    {
        glbinding::Version nearest = requestedVersion.nearest();
        return nearest > maximumVersion ? maximumVersion : nearest;
    }
    return requestedVersion;
}

int GLContextFormat::majorVersion() const
{
    return m_version.m_major;
}

int GLContextFormat::minorVersion() const
{
    return m_version.m_minor;
}

const glbinding::Version & GLContextFormat::version() const
{
    return m_version;
}

GLContextFormat::Profile GLContextFormat::profile() const
{
    return m_profile;
}

void GLContextFormat::setProfile(const GLContextFormat::Profile profile)
{
    m_profile = profile;
}

bool GLContextFormat::debugContext() const
{
    return m_debugContext;
}

void GLContextFormat::setDebugContext(const bool on)
{
    m_debugContext = on;
}

bool GLContextFormat::forwardCompatible() const
{
    return m_forwardCompatibility;
}

void GLContextFormat::setForwardCompatible(const bool on)
{
    m_forwardCompatibility = on;
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

GLContextFormat::SwapBehavior GLContextFormat::swapBehavior() const
{
    return m_swapBehavior;
}

void GLContextFormat::setSwapBehavior(const GLContextFormat::SwapBehavior behavior)
{
    m_swapBehavior = behavior;
}

bool GLContextFormat::stereo() const
{
    return m_stereo;
}

void GLContextFormat::setStereo(const bool enable)
{
    m_stereo = enable;
}

int GLContextFormat::samples() const
{
    return m_samples;
}

void GLContextFormat::setSamples(const int samples)
{
    m_samples = samples;
}

const std::string & GLContextFormat::profileString(const Profile profile)
{
    static const std::map<Profile, std::string> profileIdentifier = {
        { Profile::Core,          "Core" }
    ,   { Profile::Compatibility, "Compatibility" } 
    ,   { Profile::None,          "None" } };

    return profileIdentifier.at(profile);
}

const std::string & GLContextFormat::swapBehaviorString(const SwapBehavior swapBehavior)
{
    static const std::map<SwapBehavior, std::string> swapbIdentifier = {
        { SwapBehavior::Default,         "Default" }
    ,   { SwapBehavior::DoubleBuffering, "DoubleBuffering" }
    ,   { SwapBehavior::SingleBuffering, "SingleBuffering" } 
    ,   { SwapBehavior::TripleBuffering, "TripleBuffering" } };

    return swapbIdentifier.at(swapBehavior);
}

bool GLContextFormat::verify(const GLContextFormat & requested, const GLContextFormat & created)
{
    return
        verifyVersionAndProfile(requested, created) &&
        verifyPixelFormat(requested, created);
}

bool GLContextFormat::verify(const GLContextFormat & requested) const
{
    return verify(requested, *this);
}

bool GLContextFormat::verifyVersionAndProfile(const GLContextFormat & requested, const GLContextFormat & created)
{
    const bool sameProfiles(requested.profile() == created.profile());

    if (!sameProfiles)
    {
        warning() << "Profile mismatch for the current context: "
            << profileString(requested.profile()) << " requested, "
            << profileString(created.profile())   << " created.";
    }

    if (requested.version() != created.version())
    {
        warning() << "Version mismatch for the current context: "
            << requested.version().toString() << " requested, "
            << created.version().toString()   << " created.";

        if (requested.profile() == Profile::Core)
            return false;
    }
    return sameProfiles;
}

inline void GLContextFormat::verifyBufferSize(
    const unsigned int sizeRequested
,   const unsigned int sizeInitialized
,   const std::string & warning
,   std::vector<std::string> & issues)
{
    if (sizeRequested == sizeInitialized)
        return;

    std::stringstream ss;
    ss << warning << " size mismatch: " << sizeRequested << " requested, " << sizeInitialized << " created.";

    issues.push_back(ss.str());
}

bool GLContextFormat::verifyPixelFormat(
    const GLContextFormat & requested
,   const GLContextFormat & created)
{
    std::vector<std::string> issues;

    const bool sameSwapBehaviors(requested.swapBehavior() == created.swapBehavior());

    if (!sameSwapBehaviors)
    {
        warning() << "Swap behavior mismatch for the current context: "
            << swapBehaviorString(requested.swapBehavior()) << " requested, "
            << swapBehaviorString(created.swapBehavior())   << " created.";
    }

    if (requested.depthBufferSize())
    {
        if (!created.depthBufferSize())
            issues.push_back("- Depth Buffer requested, but none created.");
        else
            verifyBufferSize(requested.depthBufferSize(), created.depthBufferSize()
                , "- Depth Buffer", issues);
    }

    verifyBufferSize(requested.redBufferSize(),   created.redBufferSize()
        , "- Red Buffer", issues);
    verifyBufferSize(requested.greenBufferSize(), created.greenBufferSize()
        , "- Green Buffer", issues);
    verifyBufferSize(requested.blueBufferSize(),  created.blueBufferSize()
        , "- Blue Buffer", issues);
    verifyBufferSize(requested.alphaBufferSize(), created.alphaBufferSize()
        , "- Alpha Buffer", issues);

    if (requested.stencilBufferSize())
    {
        if (!created.stencilBufferSize())
            issues.push_back("- Stencil Buffer requested, but none created.");
        else
            verifyBufferSize(requested.stencilBufferSize(), created.stencilBufferSize()
                , "- Stencil Buffer", issues);
    }

    if (requested.stereo() && !created.stereo())
        issues.push_back("- Stereo Buffering requested, but not initialized.");

    if (requested.samples())
    {
        if (!created.samples())
            issues.push_back("- Sample Buffers requested, but none initialized.");
        else
            verifyBufferSize(requested.samples(), created.samples(), "- Samples ", issues);
    }

    if (issues.empty())
        return true;

    warning() << "Pixelformat mismatch for the current context:";
    for(const std::string & issue : issues)
        warning() << issue;

    return false;
}


} // namespace gloperate
