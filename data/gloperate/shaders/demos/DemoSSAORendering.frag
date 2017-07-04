
#version 140
#extension GL_ARB_explicit_attrib_location : require


const vec3 baseColor = vec3(1.0);


flat in vec3 v_normal;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec3 normal;


void main()
{
    fragColor = vec4(baseColor, 1.0);
    normal = v_normal;
}
