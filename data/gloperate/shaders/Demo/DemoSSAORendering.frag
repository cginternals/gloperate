
#version 140
#extension GL_ARB_explicit_attrib_location : require


const vec3 lightIntensity = vec3(0.9);


flat in vec3 v_normal;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec3 normal;


void main()
{
    fragColor = vec4(vec3(dot(v_normal, lightIntensity)), 1.0);
    normal = v_normal;
}
