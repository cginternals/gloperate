#version 330

layout (location = 0) in vec2 in_origin;
layout (location = 1) in vec2 in_extent;

layout (location = 2) in vec4 in_uvRect;  // vec2 lowerLeft and vec2 upperRight in glyph texture (uv)

out vec3 v_tangent;
out vec3 v_bitangent;

out vec4 v_uvRect;

void main()
{
    gl_Position = vec4(in_origin, 0.0, 1.0);

    v_tangent   = vec3(in_extent.x, 0.0, 0.0);
    v_bitangent = vec3(0.0, in_extent.y, 0.0);

    v_uvRect    = in_uvRect;
}
