
#version 140
#extension GL_ARB_explicit_attrib_location : require


const float alpha = 0.35;


uniform sampler2D transparencyKernel;
uniform sampler3D noiseKernel;
uniform vec3 color;
uniform float randVal;


in vec2 v_localPos;

layout (location = 0) out vec4 fragColor;


void main()
{
    // Making a circle from a square
    if (v_localPos.x * v_localPos.x + v_localPos.y * v_localPos.y > 1.0)
    {
        discard;
    }

    float rand = texture(noiseKernel, vec3(v_localPos * 0.5 + 0.5, randVal)).r;

    ivec2 transpSize = textureSize(transparencyKernel, 0);
    ivec2 transpIndex = ivec2(vec2(rand, alpha) * transpSize);
    bool opaque = texelFetch(transparencyKernel, transpIndex, 0).r > 0.5;

    if (!opaque)
        discard;

    fragColor = vec4(color, 1.0);
}
