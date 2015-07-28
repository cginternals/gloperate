
#include <gloperate/tools/ScreenCapturer.h>

#include <cassert>
#include <cmath>
#include <ctime>
#include <set>
#include <sys/stat.h>
#include <iomanip>

#include <glbinding/gl/enum.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/resources/ResourceManager.h>

#define CM_PER_INCH 2.54
#define INCH_PER_CM 1 / CM_PER_INCH

namespace gloperate
{


const std::string ScreenCapturer::s_ppiString = "ppi";
const std::string ScreenCapturer::s_pixelsPerCmString = "px/cm";
const std::string ScreenCapturer::s_pixelString = "pixel";
const std::string ScreenCapturer::s_inchString = "inch";
const std::string ScreenCapturer::s_cmString = "cm";


ScreenCapturer::ScreenCapturer(Painter * painter, ResourceManager & resourceManager)
:   m_painter(painter)
,   m_resourceManager(resourceManager)
,   m_viewportCapability(painter->getCapability<AbstractViewportCapability>())
,   m_framebufferCapability(painter->getCapability<AbstractTargetFramebufferCapability>())
,	m_supportedTags({ { "width", "<width>" }, { "height", "<height>" }, { "enum_num", "<enum#" }, { "year", "<year>" }, { "month", "<month>" }, { "day", "<day>" }, { "hour", "<hour>" }, { "minute", "<minute>" }, { "second", "<second>" } })
,   m_width(1920)
,   m_height(1080)
,   m_resolutionState(new ResolutionState(72, s_ppiString))
{
    assert(isApplicableTo(painter));
}

bool ScreenCapturer::isApplicableTo(Painter * painter)
{
    return painter->getCapability<AbstractViewportCapability>() != nullptr
        && painter->getCapability<AbstractTargetFramebufferCapability>() != nullptr;
}

void ScreenCapturer::initialize()
{
    m_fbo = new globjects::Framebuffer();
    m_color = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depth = new globjects::Renderbuffer();

    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);
}

void ScreenCapturer::save(const std::string & filename, const int & width, const int & height, const int & renderIterations)
{
	const int oldWidth{ m_viewportCapability->width() }, oldHeight{ m_viewportCapability->height() }, oldX{ m_viewportCapability->x() }, oldY{ m_viewportCapability->y() };
	if (width > 0 && height > 0)
		m_viewportCapability->setViewport(0, 0, width, height);

    m_color->image2D(0, gl::GL_RGBA, m_viewportCapability->width(), m_viewportCapability->height(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    // [TODO] Check for availability of depth format
    m_depth->storage(gl::GL_DEPTH_COMPONENT32, m_viewportCapability->width(), m_viewportCapability->height());

    globjects::Framebuffer * oldFbo = m_framebufferCapability->framebuffer();
    m_framebufferCapability->setFramebuffer(m_fbo);

	for (int i = 0; i < renderIterations; i++)
		m_painter->paint();

    // [TODO] handle filename
    m_resourceManager.store<globjects::Texture>(filename, m_color);

    m_framebufferCapability->setFramebuffer(oldFbo);
	if (width > 0 && height > 0)
		m_viewportCapability->setViewport(oldX, oldY, oldWidth, oldHeight);
}

void ScreenCapturer::save(const std::string & filename, bool, const int & renderIterations)
{
    save(filename, std::max(1, m_width), std::max(1, m_height), renderIterations);
}

const std::string ScreenCapturer::checkFilename(const std::string & fileName)
{
    const std::string emp("");
    std::string fileNameToCheck(fileName);
    std::string errorMessage{ "" };

    for (auto it = m_supportedTags.begin(); it != m_supportedTags.end(); it++)
    {
        if ((fileNameToCheck.find(it->second) != std::string::npos) && it->first != "enum_num")
            do
            {
                int pos = fileNameToCheck.find(it->second);
                fileNameToCheck.replace(pos, it->second.length(), emp);
            } while (fileNameToCheck.find(it->second) != std::string::npos);
    }

    if (fileNameToCheck.find(m_supportedTags["enum_num"]) != std::string::npos)
    {
        int position = fileNameToCheck.find(m_supportedTags["enum_num"]);
        fileNameToCheck.replace(position, static_cast<int>(m_supportedTags["enum_num"].length()), emp);

        std::set<char> numbers{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        bool numberFound{ false }, endFound{ false };

        while (position < fileNameToCheck.length())
        {
            if (numbers.find(fileNameToCheck.at(position)) != numbers.end())
            {
                fileNameToCheck.replace(position, 1, emp);
                numberFound = true;
            }
            else if (numberFound && fileNameToCheck.at(position) == '>')
            {
                fileNameToCheck.replace(position, 1, emp);
                endFound = true;
                break;
            }
            else
                break;
        }

        if (!endFound)
            errorMessage = "includes an incomplete <enum#> tag";
    }

    if (errorMessage == "")
    {
        if (fileNameToCheck.find_first_of("/\\*\":|?<>") != std::string::npos)
            errorMessage = "includes invalid symbols";
    }

    return errorMessage;
}


const std::map<const std::string, const std::string> & ScreenCapturer::supportedTags()
{
    return m_supportedTags;
}

std::string ScreenCapturer::dirName()
{
    return m_dirName;
}

void ScreenCapturer::setDirName(const std::string & dirName)
{
    m_dirName = dirName;
}

std::string ScreenCapturer::replaceTags(const std::string& filename, bool shouldUpdateUiFilename)
{
    std::time_t now{ 0 };
    std::tm * time{ localtime(&now) };
    std::string filenameWithReplacedTags{ filename };
    std::stringstream day, month, year, hour, minute, second;

    day << std::setw(2) << std::setfill('0') << time->tm_mday;
    month << std::setw(2) << std::setfill('0') << time->tm_mon;
    year << std::setw(2) << std::setfill('0') << time->tm_year;
    hour << std::setw(2) << std::setfill('0') << time->tm_hour;
    minute << std::setw(2) << std::setfill('0') << time->tm_min;
    second << std::setw(2) << std::setfill('0') << time->tm_sec;


    if (filenameWithReplacedTags.find(m_supportedTags["width"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["width"]), m_supportedTags["width"].length(), std::to_string(m_width));

    if (filenameWithReplacedTags.find(m_supportedTags["height"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["height"]), m_supportedTags["height"].length(), std::to_string(m_height));

    if (filenameWithReplacedTags.find(m_supportedTags["day"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["day"]), m_supportedTags["day"].length(), day.str());

    if (filenameWithReplacedTags.find(m_supportedTags["month"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["month"]), m_supportedTags["month"].length(), month.str());

    if (filenameWithReplacedTags.find(m_supportedTags["year"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["year"]), m_supportedTags["year"].length(), year.str());

    if (filenameWithReplacedTags.find(m_supportedTags["hour"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["hour"]), m_supportedTags["hour"].length(), hour.str());

    if (filenameWithReplacedTags.find(m_supportedTags["minute"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["minute"]), m_supportedTags["minute"].length(), minute.str());

    if (filenameWithReplacedTags.find(m_supportedTags["second"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["second"]), m_supportedTags["second"].length(), second.str());

    if (filenameWithReplacedTags.find(m_supportedTags["enum_num"]) != std::string::npos)
    {
        size_t position = filenameWithReplacedTags.find(m_supportedTags["enum_num"]);
        filenameWithReplacedTags.replace(position, m_supportedTags["enum_num"].length(), "");

        std::string startIndex{ extractEnumNumStartIndex(filenameWithReplacedTags, static_cast<int>(position)) };

        int index{ atoi(startIndex.c_str()) };
        filenameWithReplacedTags.replace(position, startIndex.length() + 1, std::to_string(index));

        if (shouldUpdateUiFilename)
            changeUiFilename();
    }

    return filenameWithReplacedTags;
}

std::string ScreenCapturer::buildFileName(const std::string & fileNameWithTags)
{
    std::string filename{ replaceTags(fileNameWithTags) };

    const std::string sep("/");
    const std::string suf(".png");

    std::string finalFilename = m_dirName + sep + filename + suf;

    // Increase duplicate number of fileName if file with the current duplicate count already exists
    int duplicate_count{ 2 };
    struct stat buf;
    while (stat(finalFilename.c_str(), &buf) != -1)
        finalFilename = m_dirName + sep + filename + " (" + std::to_string(duplicate_count++) + ")" + suf;

    return finalFilename;
}

std::string ScreenCapturer::extractEnumNumStartIndex(const std::string& filename, int position)
{
    std::string startIndex{ "" };

    while (filename.at(position) != '>')
    {
        startIndex += filename.at(position);
        position++;
    }

    return startIndex;
}

double ScreenCapturer::inchToPixels(double value)
{
    if (m_resolutionState->type == s_ppiString)
        value *= m_resolutionState->value;
    else if (m_resolutionState->type == s_pixelsPerCmString)
        value *= m_resolutionState->value * CM_PER_INCH;

    return value;
}

double ScreenCapturer::cmToPixels(double value)
{
    if (m_resolutionState->type == s_ppiString)
        value *= m_resolutionState->value * INCH_PER_CM;
    else if (m_resolutionState->type == s_pixelsPerCmString)
        value *= m_resolutionState->value;

    return value;
}

double ScreenCapturer::toPixels(double value, const std::string& type)
{
    if (type == s_inchString)
        value = inchToPixels(value);
    else if (type == s_cmString)
        value = cmToPixels(value);

    return value;
}

double ScreenCapturer::pixelsToCm(double value)
{
    if (m_resolutionState->type == s_ppiString)
        value *= CM_PER_INCH / m_resolutionState->value;
    else if (m_resolutionState->type == s_pixelsPerCmString)
        value /= m_resolutionState->value;

    return value;
}

double ScreenCapturer::pixelsToInch(double value)
{
    if (m_resolutionState->type == s_ppiString)
        value /= m_resolutionState->value;
    else if (m_resolutionState->type == s_pixelsPerCmString)
        value *= INCH_PER_CM / m_resolutionState->value;

    return value;
}

double ScreenCapturer::pixelsTo(double value, const std::string& type)
{
    if (type == s_inchString)
        value = pixelsToInch(value);
    else if (type == s_cmString)
        value = pixelsToCm(value);

    return value;
}

int ScreenCapturer::width()
{
    return m_width;
}

int ScreenCapturer::height()
{
    return m_height;
}

void ScreenCapturer::setWidth(int width)
{
    m_width = width;
}

void ScreenCapturer::setHeight(int height)
{
    m_height = height;
}

const ResolutionState & ScreenCapturer::resolutionState()
{
    return *m_resolutionState.get();
}

void ScreenCapturer::setResolutionState(ResolutionState & resolutionState)
{
    m_resolutionState.reset(&resolutionState);
}

void ScreenCapturer::setResolutionValue(double value)
{
    m_resolutionState->value = value;
}

void ScreenCapturer::setResolutionUnit(std::string & unit)
{
    m_resolutionState->type = unit;
}

void ScreenCapturer::createResolutionSummary()
{
    // TODO: detect unsigned long long overflow
    resolutionChanged(m_width, m_height);

    unsigned long long pixelNumber{ static_cast<unsigned long long>(m_width) * static_cast<unsigned long long>(m_height) };
    std::string unit;
    double byte;
    if (pixelNumber * 4 < 1024)
    {
        unit = "Byte";
        byte = static_cast<double>(pixelNumber)* 4;
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 2)))
    {
        unit = "KiB";
        byte = static_cast<double>(pixelNumber)* 4 / 1024;
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 3)))
    {
        unit = "MiB";
        byte = static_cast<double>(pixelNumber)* 4 / std::pow<int, int>(1024, 2);
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 4)))
    {
        unit = "GiB";
        byte = static_cast<double>(pixelNumber)* 4 / std::pow<int, int>(1024, 3);
    }
    else //if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 5)))
    {
        unit = "TiB";
        byte = static_cast<double>(pixelNumber)* 4 / std::pow<int, int>(1024, 4);
    }

    std::string summary{ std::to_string(pixelNumber) + " pixels (" + std::to_string(std::round(byte * 100) / 100) + " " + unit + " uncompressed)" };
    resolutionSummaryChanged(summary);
}


} // namespace gloperate
