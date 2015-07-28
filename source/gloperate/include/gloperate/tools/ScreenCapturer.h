
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


struct ResolutionState {
    ResolutionState(double d, std::string s)
        : value( d )
        , type( s )
    {
    }

    double value;
    std::string type;
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
    ScreenCapturer(Painter * painter, ResourceManager & resourceManager);

    static bool isApplicableTo(Painter * painter);

    void initialize();

    void save(const std::string & filename, const int & width = 0, const int & height = 0, const int & renderIterations = 1);
    const std::string checkFilename(const std::string & filename);
    const std::map<const std::string, const std::string> & supportedTags();

    std::string dirName();
    void setDirName(const std::string & dirName);
    std::string replaceTags(const std::string& filename, int width, int height, bool shouldUpdateUiFilename = true);
    std::string buildFileName(const std::string & fileNameWithTags, int width, int height);
    std::string extractEnumNumStartIndex(const std::string& filename, int position);

    int width();
    int height();
    void setWidth(int width);
    void setHeight(int height);
    const ResolutionState & resolutionState();
    void setResolutionState(ResolutionState & resolutionState);
    void setResolutionValue(double value);
    void setResolutionUnit(std::string & unit);

    double inchToPixels(double value);
    double cmToPixels(double value);
    double toPixels(double value, const std::string& type);
    double pixelsToCm(double value);
    double pixelsToInch(double value);
    double pixelsTo(double value, const std::string& type);

    void createResolutionSummary();


public:
    signalzeug::Signal<> changeUiFilename;
    signalzeug::Signal<const std::string&> resolutionSummaryChanged;
    signalzeug::Signal<const int, const int> resolutionChanged;


public:
    static const std::string s_ppiString;
    static const std::string s_pixelsPerCmString;
    static const std::string s_pixelString;
    static const std::string s_inchString;
    static const std::string s_cmString;


protected:
    Painter * m_painter;
    ResourceManager & m_resourceManager;
    AbstractViewportCapability * m_viewportCapability;
    AbstractTargetFramebufferCapability * m_framebufferCapability;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Texture> m_color;
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;

    std::map<const std::string, const std::string> m_supportedTags;
    std::string m_dirName;

    int m_width;
    int m_height;
    std::unique_ptr<ResolutionState> m_resolutionState;
};


} // namespace gloperate
