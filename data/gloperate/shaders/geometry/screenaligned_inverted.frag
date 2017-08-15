
#version 140
#extension GL_ARB_explicit_attrib_location : require


uniform sampler2D source;


in vec2 v_uv;

layout (location = 0) out vec4 fragColor;


void main()
{
    fragColor = texture(source, vec2(v_uv.x, 1.0 - v_uv.y));
}
