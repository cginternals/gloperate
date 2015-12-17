#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D gradients;
uniform int gradientIndex;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
    ivec2 size = textureSize(gradients, 0);
    fragColor = vec4(texelFetch(gradients, ivec2(int(v_uv.x * float(size.x)), gradientIndex), 0).rgb, 1.0);
}
