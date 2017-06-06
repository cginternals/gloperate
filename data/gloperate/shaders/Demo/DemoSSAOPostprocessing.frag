#version 140
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_shading_language_include : require

#include </gloperate/shaders/lighting/ssao.glsl>

const vec3 ssaoColor = vec3(0.0);
const float farZ = 10.0;
const float ssaoRadius = 0.2;
const float ssaoIntensity = 0.5;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler1D ssaoKernelTexture;
uniform sampler3D ssaoNoiseTexture;

uniform mat4 projectionMatrix;
uniform mat4 projectionInverseMatrix;
uniform mat3 normalMatrix;

in vec2 v_uv;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec3 baseColor = texture(colorTexture, v_uv).rgb;

    vec3 ssaoFactor = ssao(
        v_uv,
        ssaoColor,
        farZ,
        ssaoRadius,
        ssaoIntensity,
        ssaoKernelTexture,
        ssaoNoiseTexture,
        depthTexture,
        normalTexture,
        projectionMatrix,
        projectionInverseMatrix,
        normalMatrix
    );

    fragColor = vec4(baseColor * ssaoFactor, 1.0);
}
