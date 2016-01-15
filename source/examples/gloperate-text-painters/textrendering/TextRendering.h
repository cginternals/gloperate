
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>


namespace gloperate
{
    class AbstractViewportCapability;
    class AbstractTargetFramebufferCapability;
    class ResourceManager;
}

namespace gloperate_text
{
    class FontFace;
}


class TextRendering : public gloperate::Painter
{
public:
    TextRendering(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo);
    virtual ~TextRendering();


protected:
    virtual void onInitialize();
    virtual void onPaint();


protected:
    gloperate::ResourceManager & m_resourceManager;
    std::string m_dataPath;

    /* Capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;

    /* Data */
    globjects::ref_ptr<gloperate_text::FontFace> m_fontFace;
};
