
#pragma once


#include <string>

#include <signalzeug/Signal.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Represent ppi or pixel per cm unit and value
*/
struct ResolutionState {
    /**
    *  @brief
    *    Initializer list Constructor
    *
    *  @param[in] d
    *    ppi or pixel per cm value
    *  @param[in] s
    *    unit type, either "ppi" or "px/cm"
    */
    ResolutionState(double d, std::string s)
        : value( d )
        , type( s )
    {
    }

    double value;       ///< value of ppi / px/cm
    std::string type;   ///< resolution type: either "ppi" or "px/cm"
};


class Painter;
class ResourceManager;
class AbstractViewportCapability;
class AbstractTargetFramebufferCapability;


/**
*  @brief
*    Tool to export images (screenshots) from a painter
*/
class GLOPERATE_API ScreenCapturer
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] painter
    *    Currently loaded painter from viewer, null_ptr is allowed
    *  @param[in] resourceManager
    *    Resource Manager from viewer
    */
    ScreenCapturer(Painter * painter, ResourceManager & resourceManager);

    /**
    *  @brief
    *    Add plugin search path
    *
    *  @param[in] painter
    *    Currently displayed painter
    *
    *  @return
    *    'true' if painter has the necessary capabilies to export 
    *    a picture, else 'false'
    */
    static bool isApplicableTo(Painter * painter);

    /**
    *  @brief
    *    Initialize the necessary buffers for image rendering
    */
    void initialize();

    /**
    *  @brief
    *    Save image of currently displayed painter to given filename
    *
    *  @param[in] filename
    *    Filename with absolute path
    *  @param[in] width
    *    Width of the image to save, default 0
    *  @param[in] height
    *    Height of the image to save, default 0
    *  @param[in] renderIterations
    *    Times the image should be rendered till saved, default 1
    *
    *  @remarks
    *    Render to new buffers with size regarding to given width and height.
    *    Restore and display former buffers when done.
    */
    void save(const std::string & filename, const int & width = 0, const int & height = 0, const int & renderIterations = 1);

    /**
    *  @brief
    *    Save image of currently displayed painter to given filename
    *
    *  @param[in] filename
    *    Filename with absolute path
    *  @param[in] renderIterations
    *    Times the image should be rendered till saved, default 1
    *
    *  @remarks
    *    Render to new buffers with size regarding to stored with and height.
    *    Minimum resolution rendered is 1x1.
    *    Restore and display former buffers when done.
    */
    void save(const std::string & filename, bool, const int & renderIterations = 1);

    /**
    *  @brief
    *    Check if filename is correct
    *
    *  @param[in] filename
    *    Filename to check
    *
    *  @return
    *    Error message, empty if filename is correct
    *
    *  @remarks
    *    Check if filename contains only supported tags and allowed characters.
    */
    const std::string checkFilename(const std::string & filename);

    /**
    *  @brief
    *    Provide supported tags
    *
    *  @return
    *    Map of supported tags containg their name and the actual tag
    */
    const std::map<const std::string, const std::string> & supportedTags();

    /**
    *  @brief
    *    Provide the current dirname to save files in
    *
    *  @return
    *    Current dirname
    */
    std::string dirName();

    /**
    *  @brief
    *    Set current dirname to save files in
    *
    *  @param[in] dirName
    *    Name of a existing directory
    */
    void setDirName(const std::string & dirName);

    /**
    *  @brief
    *    Replace tags in filename
    *
    *  @param[in] filename
    *    Filename
    *  @param[in] shouldUpdateUiFilename
    *    Should be true if <enum#..> tag is used
    *
    *  @return
    *    Filename with tags replaced
    *
    *  @remarks
    *    Replace all supported tags with their indicated replacements.
    *    If enumerater increases and ui filename should be updated,
    *    emits signal to do so.
    */
    std::string replaceTags(const std::string& filename, bool shouldUpdateUiFilename = true);

    /**
    *  @brief
    *    Build absolute filename
    *
    *  @param[in] filename
    *    Filename that might contain tags
    *
    *  @return
    *    Absolute filepath
    *
    *  @remarks
    *    Call method to replace tags and add current directory and file extension.
    */
    std::string buildFileName(const std::string & fileNameWithTags);

    /**
    *  @brief
    *    Build absolute filename
    *
    *  @param[in] filename
    *    Filename containing an <enum#..> tag
    *  @param[in] position
    *    Position in filename string right behind <enum#
    *
    *  @return
    *    Start index of enueration
    *
    *  @remarks
    *    Extracts string between '<enum#' and next following '>'.
    */
    std::string extractEnumNumStartIndex(const std::string& filename, int position);

    /**
    *  @brief
    *    Getter of m_width
    *
    *  @return
    *    Value of m_width
    */
    int width();

    /**
    *  @brief
    *    Getter of m_height
    *
    *  @return
    *    Value of m_height
    */
    int height();

    /**
    *  @brief
    *    Setter of m_width
    *
    *  @param[in] width
    *    A width
    */
    void setWidth(int width);

    /**
    *  @brief
    *    Setter of m_height
    *
    *  @param[in] height
    *    A height
    */
    void setHeight(int height);

    /**
    *  @brief
    *    Getter of m_resolutionState
    *
    *  @return
    *    Value of m_resolutionState
    */
    const ResolutionState & resolutionState();

    /**
    *  @brief
    *    Setter of m_resolutionState
    *
    *  @param[in] resolutionState
    *    A ResolutionState
    */
    void setResolutionState(ResolutionState & resolutionState);

    /**
    *  @brief
    *    Setter of m_resolutionState's value
    *
    *  @param[in] value
    *    A ResolutionState value
    */
    void setResolutionValue(double );

    /**
    *  @brief
    *    Setter of m_resolutionState's unit
    *
    *  @param[in] unit
    *    A ResolutionState unit
    */
    void setResolutionUnit(std::string & unit);

    /**
    *  @brief
    *    Convert inch to pixels
    *
    *  @param[in] value
    *    A inch value
    *
    *  @return
    *    Pixel value
    */
    double inchToPixels(double value);

    /**
    *  @brief
    *    Convert cm to pixels
    *
    *  @param[in] value
    *    A cm value
    *
    *  @return
    *    Pixel value
    */
    double cmToPixels(double value);

    /**
    *  @brief
    *    Convert inch or cm to pixels
    *
    *  @param[in] value
    *    A inch or cm value
    *  @param[in] type
    *    Type of value
    *
    *  @return
    *    Pixel value
    */
    double toPixels(double value, const std::string& type);

    /**
    *  @brief
    *    Convert pixels to cm
    *
    *  @param[in] value
    *    A pixel value
    *
    *  @return
    *    Cm value
    */
    double pixelsToCm(double value);

    /**
    *  @brief
    *    Convert pixels to inch
    *
    *  @param[in] value
    *    A pixel value
    *
    *  @return
    *    Inch value
    */
    double pixelsToInch(double value);

    /**
    *  @brief
    *    Convert pixels to inch or cm
    *
    *  @param[in] value
    *    A pixel value
    *  @param[in] type
    *    Type to convert to
    *
    *  @return
    *    Cm of inch value, depending on type
    */
    double pixelsTo(double value, const std::string& type);

    /**
    *  @brief
    *    Create a string containing the pixel count and the needed uncompressed hard drive space of a image
    *
    *  @remarks
    *    Emit signal resolutionSummaryChanged with the summary and
    *    emit resolutionChanged with the current resolution.
    */
    void createResolutionSummary();


public:
    signalzeug::Signal<> changeUiFilename;                              ///< Emitted if displayed filename should be updated
    signalzeug::Signal<const std::string&> resolutionSummaryChanged;    ///< Emitted if new resolution summary is avaiable
    signalzeug::Signal<const int, const int> resolutionChanged;         ///< Emitted if new resolution is available


public:
    static const std::string s_ppiString;           ///< Display name for ppi
    static const std::string s_pixelsPerCmString;   ///< Display name for pixels per cm
    static const std::string s_pixelString;         ///< Display name for pixel
    static const std::string s_inchString;          ///< Display name for inch
    static const std::string s_cmString;            ///< Display name for cm


protected:
    Painter * m_painter;                                            ///< Painter used in the viewer
    ResourceManager & m_resourceManager;                            ///< Resource Manager user in the viewer
    AbstractViewportCapability * m_viewportCapability;              ///< ViewportCapability of painter
    AbstractTargetFramebufferCapability * m_framebufferCapability;  ///< TargetFramebufferCapability of painter

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;       ///< Framebuffer for image rendering
    globjects::ref_ptr<globjects::Texture> m_color;         ///< Colorbuffer for image rendering
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;    ///< Renderbuffer for image rendering

    std::map<const std::string, const std::string> m_supportedTags;     ///< Supported tags for filenames
    std::string m_dirName;                                              ///< Current displayed directory name

    int m_width;                                            ///< Width of image saved by save
    int m_height;                                           ///< Height of image saved by save
    std::unique_ptr<ResolutionState> m_resolutionState;     ///< Resolution used to convert inch and cm in pixel
};


} // namespace gloperate
