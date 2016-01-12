#pragma once

#include <vector>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/primitives/VertexDrawable.h>

#include <gloperate-text/gloperate-text_api.h>

#include <gloperate-text/geometry/IndirectGlyphVertex.h>


namespace gloperate_text
{


class GLOPERATE_TEXT_API IndirectGlyphVertexCloud
{
public:
    IndirectGlyphVertexCloud();

    void verticesChanged();

    gloperate::VertexDrawable * drawable();
    const gloperate::VertexDrawable * drawable() const;

public:
    std::vector<IndirectGlyphVertex> vertices;
    globjects::ref_ptr<globjects::Texture> glyphTexture;
    globjects::ref_ptr<globjects::Texture> glyphToCoordinateTexture;
    globjects::ref_ptr<globjects::Texture> glyphConfigurationTexture;

protected:
    globjects::ref_ptr<gloperate::VertexDrawable> m_drawable;

    void updateDrawable();
};


} // namespace gloperate_text
