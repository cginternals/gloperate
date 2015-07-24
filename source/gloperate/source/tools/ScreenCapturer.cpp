
#include <gloperate/tools/ScreenCapturer.h>

#include <cassert>
#include <set>
#include <ctime>
#include <sys/stat.h>
#include <iomanip>

#include <glbinding/gl/enum.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/resources/ResourceManager.h>


namespace gloperate
{


ScreenCapturer::ScreenCapturer(Painter * painter, ResourceManager & resourceManager)
:   m_painter(painter)
,   m_resourceManager(resourceManager)
,   m_viewportCapability(painter->getCapability<AbstractViewportCapability>())
,   m_framebufferCapability(painter->getCapability<AbstractTargetFramebufferCapability>())
,	m_supportedTags({ { "width", "<width>" }, { "height", "<height>" }, { "enum_num", "<enum#" }, { "year", "<year>" }, { "month", "<month>" }, { "day", "<day>" }, { "hour", "<hour>" }, { "minute", "<minute>" }, { "second", "<second>" } })
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

std::string ScreenCapturer::replaceTags(const std::string& filename, int width, int height, bool shouldUpdateUiFilename)
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
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["width"]), m_supportedTags["width"].length(), std::to_string(width));

    if (filenameWithReplacedTags.find(m_supportedTags["height"]) != std::string::npos)
        filenameWithReplacedTags.replace(filenameWithReplacedTags.find(m_supportedTags["height"]), m_supportedTags["height"].length(), std::to_string(height));

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

std::string ScreenCapturer::buildFileName(const std::string & fileNameWithTags, int width, int height)
{
    std::string filename{ replaceTags(fileNameWithTags, width, height) };

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


} // namespace gloperate
