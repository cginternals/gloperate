
#pragma once

#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/AbstractStage.h>

#include <gloperate-text/gloperate-text_api.h>

#include <gloperate-text/geometry/GlyphVertexCloud.h>


namespace gloperate_text
{


enum class Encoding : unsigned int;

class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::AbstractStage
{
public:
    GlyphRenderStage();
    virtual ~GlyphRenderStage();

    gloperate::InputSlot<std::vector<std::string>> strings;
    gloperate::InputSlot<Encoding> encoding;

    gloperate::Data<GlyphVertexCloud> vertices;

protected:
    virtual void initialize() override;
    virtual void process() override;

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
};


} // namespace gloperate_text
