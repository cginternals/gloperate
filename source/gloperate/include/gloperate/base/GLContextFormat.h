
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
    *    the requested version, if it is lower or equal to the
    *    maximum version. Otherwise, the maximum version is returned.
    */
    static glbinding::Version validateVersion(
        const glbinding::Version & requested
      , const glbinding::Version & maximum);


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

    /**
    *  @brief
    *    Get OpenGL profile
    *
    *  @return
    *    OpenGL profile (default: None)
    */
    Profile profile() const;

    /**
    *  @brief
    *    Set OpenGL profile
    *
    *  @param[in] profile
    *    OpenGL profile
    */
    void setProfile(Profile profile);

    /**
    *  @brief
    *    Get forward compatibility mode
    *
    *  @return
    *    Forward compatibility mode (default: false)
    */
    bool forwardCompatible() const;

    /**
    *  @brief
    *    Set forward compatibility mode
    *
    *  @param[in] on
    *    Forward compatibility mode
    */
    void setForwardCompatible(bool on);

    /**
    *  @brief
    *    Get debug context mode
    *
    *  @return
    *    Debug context mode (default: false)
    */
    bool debugContext() const;

    /**
    *  @brief
    *    Set debug context mode
    *
    *  @param[in] on
    *    Debug context mode
    */
    void setDebugContext(bool on);

    /**
    *  @brief
    *    Get buffer size for red color channel
    *
    *  @return
    *    Buffer size (default: -1)
    */
    int redBufferSize() const;

    /**
    *  @brief
    *    Set buffer size for red color channel
    *
    *  @param[in] size
    *    Buffer size
    */
    void setRedBufferSize(int size);

    /**
    *  @brief
    *    Get buffer size for green color channel
    *
    *  @return
    *    Buffer size (default: -1)
    */
    int greenBufferSize() const;

    /**
    *  @brief
    *    Set buffer size for green color channel
    *
    *  @param[in] size
    *    Buffer size
    */
    void setGreenBufferSize(int size);

    /**
    *  @brief
    *    Get buffer size for blue color channel
    *
    *  @return
    *    Buffer size (default: -1)
    */
    int blueBufferSize() const;

    /**
    *  @brief
    *    Set buffer size for blue color channel
    *
    *  @param[in] size
    *    Buffer size
    */
    void setBlueBufferSize(int size);

    /**
    *  @brief
    *    Get buffer size for alpha color channel
    *
    *  @return
    *    Buffer size (default: -1)
    */
    int alphaBufferSize() const;

    /**
    *  @brief
    *    Set buffer size for alpha color channel
    *
    *  @param[in] size
    *    Buffer size
    */
    void setAlphaBufferSize(int size);

    /**
    *  @brief
    *    Get depth buffer size
    *
    *  @return
    *    Buffer size (default: 24)
    */
    int depthBufferSize() const;

    /**
    *  @brief
    *    Set depth buffer size
    *
    *  @param[in] size
    *    Buffer size
    */
    void setDepthBufferSize(int size);

    /**
    *  @brief
    *    Get stencil buffer size
    *
    *  @return
    *    Buffer size (default: 0)
    */
    int stencilBufferSize() const;

    /**
    *  @brief
    *    Set stencil buffer size
    *
    *  @param[in] size
    *    Buffer size
    */
    void setStencilBufferSize(int size);

    /**
    *  @brief
    *    Get stereo mode
    *
    *  @return
    *    Stereo mode (default: false)
    */
    bool stereo() const;

    /**
    *  @brief
    *    Set stereo mode
    *
    *  @param[in] on
    *    Stereo mode
    */
    void setStereo(bool on);

    /**
    *  @brief
    *    Get samples
    *
    *  @return
    *    Number of samples (default: -1)
    */
    int samples() const;

    /**
    *  @brief
    *    Set samples
    *
    *  @param[in] samples
    *    Number of samples
    */
    void setSamples(int samples);

    /**
    *  @brief
    *    Get swap behavior
    *
    *  @return
    *    Swap behavior (default: DoubleBuffering)
    */
    SwapBehavior swapBehavior() const;

    /**
    *  @brief
    *    Set swap behavior
    *
    *  @param[in] behavior
    *    Swap behavior
    */
    void setSwapBehavior(SwapBehavior behavior);

    /**
    *  @brief
    *    Compare OpenGL context formats against a requested format
    *
    *  @param[in] requested
    *    OpenGL context format
    *
    *  @return
    *    'true' if context formats are equal, else 'false'
    *
    *  @remarks
    *    This function compares the OpenGL context version, profile,
    *    options, and pixel formats. Any found issues are logged to
    *    globjects::warning.
    */
    bool verify(const GLContextFormat & requested) const;


protected:
    /**
    *  @brief
    *    Compare OpenGL context version and profile against a requested format
    *
    *  @param[in] requested
    *    OpenGL context format
    *
    *  @return
    *    'true' if context versions and profiles are equal, else 'false'
    *
    *  @remarks
    *    Any found issues are logged to globjects::warning.
    */
    bool verifyVersionAndProfile(const GLContextFormat & requested) const;

    /**
    *  @brief
    *    Compare buffer sizes and options against a requested format
    *
    *  @param[in] requested
    *    OpenGL context format
    *
    *  @return
    *    'true' if context options are equal, else 'false'
    *
    *  @remarks
    *    Any found issues are logged to globjects::warning.
    */
    bool verifyPixelFormat(const GLContextFormat & requested) const;

    /**
    *  @brief
    *    Compare buffer sizes
    *
    *  @param[in] sizeRequested
    *    Requested buffer size
    *  @param[in] sizeInitialized
    *    Found buffer size
    *  @param[in] warning
    *    Warning message that is issued when buffer sizes are not equal
    *  @param[out] issues
    *    List of warnings
    *
    *  @remarks
    *    If the requested and found sizes are not equal, the function will
    *    add the specified warning to the output list of warnings.
    */
    void verifyBufferSize(
        unsigned int sizeRequested
      , unsigned int sizeInitialized
      , const std::string & warning
      , std::vector<std::string> & issues) const;


protected:
    glbinding::Version m_version;       ///< OpenGL version (major, minor)
    Profile            m_profile;       ///< OpenGL profile (core, compatibility, none)

    bool m_forwardCompatibility;        ///< Forward compatibility mode
    bool m_debugContext;                ///< Debug context mode

    unsigned int m_redBufferSize;       ///< Buffer size for red color channel
    unsigned int m_greenBufferSize;     ///< Buffer size for green color channel
    unsigned int m_blueBufferSize;      ///< Buffer size for blue color channel
    unsigned int m_alphaBufferSize;     ///< Buffer size for alpha color channel
    unsigned int m_depthBufferSize;     ///< Depth buffer size
    unsigned int m_stencilBufferSize;   ///< Stencil buffer size
    bool         m_stereo;              ///< Stereo mode
    unsigned int m_samples;             ///< Number of samples

    SwapBehavior m_swapBehavior;        ///< Swap behavior
};


} // namespace gloperate
