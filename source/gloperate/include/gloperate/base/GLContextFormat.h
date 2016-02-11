
#pragma once


#include <vector>
#include <string>

#include <glbinding/Version.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Description of an OpenGL context format
*/
class GLOPERATE_API GLContextFormat
{
public:
    /**
    *  @brief
    *    OpenGL profile
    */
    enum class Profile
    {
        Core          ///< Functionality deprecated in OpenGL version 3.0 is not available.
      , Compatibility ///< Functionality from earlier OpenGL versions is available.
      , None          ///< Version < 3.2, otherwise identical to Core
    };

    /**
    *  @brief
    *    Buffer swap behavior
    */
    enum class SwapBehavior
    {
        Default         ///< The default swap behaviour (of the platform).
      , SingleBuffering ///< Might result in flickering when is done directly to screen without an intermediate offscreen buffer.
      , DoubleBuffering ///< Rendering is done to the back buffer, which is then swapped with the front buffer.
      , TripleBuffering ///< Sometimes used in order to decrease the risk of skipping a frame when the rendering rate is just barely keeping up with the screen refresh rate.
    };


public:
    /**
    *  @brief
    *    Get OpenGL profile as string
    *
    *  @param[in] profile
    *    Profile
    *
    *  @return
    *    String representation
    */
    static const std::string & profileString(Profile profile);

    /**
    *  @brief
    *    Get swap behaviour as string
    *
    *  @param[in] swapBehavior
    *    Swap behavior
    *
    *  @return
    *    String representation
    */
    static const std::string & swapBehaviorString(SwapBehavior swapBehavior);

    /**
    *  @brief
    *    Validate OpenGL context version against a maximum version
    *
    *  @param[in] requested
    *    OpenGL version
    *  @param[in] maximum
    *    OpenGL version
    *
    *  @return
    *    OpenGL version
    *
    *  @remarks
    *    If the requested version is not valid, the nearest valid
    *    version is tried to be found. The function will then return
    *    the requested version if it is lower or equal to the
    *    maximum version, or else the maximum version.
    */
    static glbinding::Version validateVersion(
        const glbinding::Version & requested
      , const glbinding::Version & maximum);

    /**
    *  @brief
    *    Compare OpenGL context version against a request version
    *
    *  @param[in] requested
    *    OpenGL version
    *  @param[in] created
    *    OpenGL version
    *
    *  @return
    *    'true' if context versions are equal, else 'false'
    *
    *  @remarks
    *    This function compares the OpenGL context version, profile,
    *    options, and pixel formats. Any found issues are logged to
    *    globjects::warning.
    */
    static bool verify(const GLContextFormat & requested, const GLContextFormat & created);


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @remarks
    *    Will create a default context format (4.5 Core)
    */
    GLContextFormat();

    /**
    *  @brief
    *    Destructor
    *
    *  @remarks
    *    Will create a default context format (4.5 Core)
    */
    ~GLContextFormat();

    /**
    *  @brief
    *    Get OpenGL version
    *
    *  @return
    *    OpenGL version
    */
    const glbinding::Version & version() const;

    /**
    *  @brief
    *    Get OpenGL major version
    *
    *  @return
    *    Major version
    */
    int majorVersion() const;

    /**
    *  @brief
    *    Get OpenGL minor version
    *
    *  @return
    *    Minor version
    */
    int minorVersion() const;

    /**
    *  @brief
    *    Set OpenGL version
    *
    *  @param[in] version
    *    OpenGL version
    *
    *  @remarks
    *    For major and minor parameters only valid version pairs are allowed.
    *    On invalid pairs, the nearest major and minor are set.
    */
    void setVersion(const glbinding::Version & version);

    /**
    *  @brief
    *    Set OpenGL version
    *
    *  @param[in] majorVersion
    *    OpenGL major version
    *  @param[in] minorVersion
    *    OpenGL minor version
    *
    *  @remarks
    *    For major and minor parameters only valid version pairs are allowed.
    *    On invalid pairs, the nearest major and minor are set.
    */
    void setVersion(unsigned int majorVersion, unsigned int minorVersion);

    Profile profile() const;
    void setProfile(Profile profile);

    bool forwardCompatible() const;
    void setForwardCompatible(bool on);

    bool debugContext() const;
    void setDebugContext(bool on);

    int redBufferSize() const;
    void setRedBufferSize(int size);

    int greenBufferSize() const;
    void setGreenBufferSize(int size);

    int blueBufferSize() const;
    void setBlueBufferSize(int size);

    int alphaBufferSize() const;
    void setAlphaBufferSize(int size);

    int depthBufferSize() const;
    void setDepthBufferSize(int size);

    int stencilBufferSize() const;
    void setStencilBufferSize(int size);

    bool stereo() const;
    void setStereo(bool enable);

    int samples() const;
    void setSamples(int samples);

    SwapBehavior swapBehavior() const;
    void setSwapBehavior(SwapBehavior behavior);

    bool verify(const GLContextFormat & requested) const;


protected:
    /** Compares (logged if erroneous) version and profile between both formats
    */
    static bool verifyVersionAndProfile(const GLContextFormat & requested, const GLContextFormat & current);

    /** Compares (logged if erroneous) buffer sizes and more between both formats
    */
    static bool verifyPixelFormat(const GLContextFormat & requested, const GLContextFormat & current);

    /** Used as inline by verifyPixelFormat 
    */
    static void verifyBufferSize(
        unsigned int sizeRequested
      , unsigned int sizeInitialized
      , const std::string & warning
      , std::vector<std::string> & issues);


protected:
    glbinding::Version m_version;
    Profile m_profile;
    bool m_forwardCompatibility;
    bool m_debugContext;

    unsigned int m_redBufferSize;
    unsigned int m_greenBufferSize;
    unsigned int m_blueBufferSize;
    unsigned int m_alphaBufferSize;
    unsigned int m_depthBufferSize;
    unsigned int m_stencilBufferSize;
    bool         m_stereo;
    unsigned int m_samples;

    SwapBehavior m_swapBehavior;
};


} // namespace gloperate
