
#include <gloperate/tools/ScreenCapturer.h>

#include <cassert>
#include <set>

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
,	m_supportedTags({ { "width", "<width>" }, { "height", "<height>" }, { "enum_num", "<enum#" }, { "year", "<year>" }, { "month", "<month>" }, { "day", "<day>" }, { "hour", "<hour>" }, { "minute", "<minute>" }, { "second", "<second>" }, { "millisec", "<millisecond>" } })
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


} // namespace gloperate
