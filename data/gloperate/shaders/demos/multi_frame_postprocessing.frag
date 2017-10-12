
#version 150
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_shading_language_include : require

#include </gloperate/shaders/lighting/ssao.glsl>


const vec3 ssaoColor = vec3(0.0);
const float farZ = 10.0;


uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler1D ssaoKernelTexture;
uniform sampler2DArray ssaoNoiseTexture;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

uniform int currentFrame;
uniform float ssaoRadius;
uniform float ssaoIntensity;

uniform bool useSSAO;


in vec2 v_uv;

layout (location = 0) out vec4 fragColor;


void main()
{
    vec3 baseColor = texture(colorTexture, v_uv).rgb;

    float ssaoFactor = 1.0;

    // Run SSAO calculation
    if (useSSAO)
    {
        ssaoFactor = ssao(
            v_uv,
            depthTexture,
            normalTexture,
            ssaoNoiseTexture,
            ssaoKernelTexture,
            modelViewProjectionMatrix,
            inverse(modelViewProjectionMatrix),
            ssaoRadius,
            currentFrame
        );
    }

    // Apply SSAO
    fragColor = vec4(baseColor * mix(1.0, ssaoFactor, ssaoIntensity), 1.0);
}
