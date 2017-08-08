
#version 140
#extension GL_ARB_explicit_attrib_location : require


const vec4 baseColor = vec4(1.0, 1.0, 1.0, 0.65);


uniform sampler2D transparencyKernel;

uniform int currentFrame;

uniform bool useTransparency;


in vec4 v_position;
flat in vec3 v_normal;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec3 normal;


void main()
{
    // produce cheap, high frequency noise
    float rand = dot(v_position, vec4(1024));

    float alpha = baseColor.a;
    ivec2 transpSize = textureSize(transparencyKernel, 0);
    ivec2 transpIndex = ivec2(vec2(rand, alpha) * transpSize + ivec2(currentFrame, 0));
    bool opaque = texelFetch(transparencyKernel, transpIndex % transpSize, 0).r > 0.5;

    if (alpha == 1.0)
        opaque = true;

    if (!opaque && useTransparency)
        discard;

    fragColor = vec4(baseColor.rgb, 1.0);
    normal = v_normal;
}
