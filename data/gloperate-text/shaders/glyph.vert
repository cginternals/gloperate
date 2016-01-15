#version 330

layout (location=0) in vec3 in_coord;
layout (location=1) in vec3 in_tangent;
layout (location=2) in vec3 in_bitangent;
layout (location=3) in vec2 in_glyphStart;
layout (location=4) in vec2 in_glyphEnd;

out vec3 v_tangent;
out vec3 v_bitangent;
out vec2 v_glyphStart;
out vec2 v_glyphEnd;

void main()
{
    gl_Position = vec4(in_coord, 1.0);
    v_tangent = in_tangent;
    v_bitangent = in_bitangent;
    v_glyphStart = in_glyphStart;
    v_glyphEnd = in_glyphEnd;
}
