#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <gloperate-text/gloperate-text_api.h>

namespace gloperate_text
{


class GLOPERATE_TEXT_API GlyphVertex
{
public:
    GlyphVertex();

public:
    glm::vec3 worldLlf;
    glm::vec3 worldUrb;
    glm::vec3 normal;
    glm::vec2 glyphStart;
    glm::vec2 glyphEnd;
};


} // namespace gloperate_text
