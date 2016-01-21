#version 330

layout (location=0) in vec2 in_worldStart;
layout (location=1) in vec2 in_worldEnd;
layout (location=2) in vec2 in_glyphStart;
layout (location=3) in vec2 in_glyphEnd;

out vec3 v_tangent;
out vec3 v_bitangent;
out vec2 v_glyphStart;
out vec2 v_glyphEnd;

void main()
{
    gl_Position = vec4(in_worldStart, 0.0, 1.0);
    v_tangent = vec3(in_worldEnd.x - in_worldStart.x, 0.0, 0.0);
    v_bitangent = vec3(0.0, in_worldEnd.y - in_worldStart.y, 0.0);
    v_glyphStart = in_glyphStart;
    v_glyphEnd = in_glyphEnd;
}
