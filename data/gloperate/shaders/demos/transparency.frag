
#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_shading_language_include : require

#include </gloperate/shaders/util/random.glsl>

uniform sampler2D transparencyKernel;

uniform int currentFrame;


flat in vec4 v_color;
in vec2 v_localPos;

layout (location = 0) out vec4 fragColor;


void main()
{
    // Making a circle from a square
    float r2 = v_localPos.x * v_localPos.x + v_localPos.y * v_localPos.y;
    if (r2 > 1.0)
    {
        discard;
    }

    float rand = random(vec3(v_localPos, currentFrame));

    float alpha = v_color.a;
    ivec2 transpSize = textureSize(transparencyKernel, 0);
    ivec2 transpIndex = ivec2(vec2(rand, alpha) * transpSize) + ivec2(currentFrame, 0);
    bool opaque = texelFetch(transparencyKernel, transpIndex % transpSize, 0).r > 0.5;

    if (!opaque)
        discard;

    fragColor = vec4(v_color.rgb, 1.0);
}
