#pragma once

#include <glm/vec2.hpp>

#include <gloperate-text/gloperate-text_api.h>

namespace gloperate_text
{


class GLOPERATE_TEXT_API GlyphVertex
{
public:
    GlyphVertex();

public:
    glm::vec2 worldStart;
    glm::vec2 worldEnd;
    glm::vec2 glyphStart;
    glm::vec2 glyphEnd;
};


} // namespace gloperate_text
