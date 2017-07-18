
#version 140
#extension GL_ARB_explicit_attrib_location : require


const float alpha = 0.35;


uniform sampler2D transparencyKernel;
uniform sampler2DArray noiseKernel;
uniform vec4 color;
uniform int frameCounter;


in vec2 v_localPos;

layout (location = 0) out vec4 fragColor;


void main()
{
    // Making a circle from a square
    if (v_localPos.x * v_localPos.x + v_localPos.y * v_localPos.y > 1.0)
    {
        discard;
    }

    float rand = texture(noiseKernel, vec3(v_localPos * 0.5 + 0.5, frameCounter)).r;

    float alpha = color.a;
    ivec2 transpSize = textureSize(transparencyKernel, 0);
    ivec2 transpIndex = ivec2(vec2(rand, alpha) * transpSize) + ivec2(frameCounter, 0);
    bool opaque = texelFetch(transparencyKernel, transpIndex % transpSize, 0).r > 0.5;

    if (!opaque)
        discard;

    fragColor = vec4(color.rgb, 1.0);
}
