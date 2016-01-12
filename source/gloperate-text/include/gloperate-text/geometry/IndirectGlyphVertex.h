#pragma once

#include <cstddef>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <gloperate-text/gloperate-text_api.h>

namespace gloperate_text
{


class GLOPERATE_TEXT_API IndirectGlyphVertex
{
public:
    IndirectGlyphVertex();

public:
    glm::vec3 worldLlf;
    glm::vec3 worldUrb;
    glm::vec3 normal;
    std::size_t index;
};


} // namespace gloperate_text
