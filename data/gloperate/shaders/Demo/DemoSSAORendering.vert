#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 viewProjectionMatrix;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

flat out vec3 v_normal;

void main()
{
    gl_Position = viewProjectionMatrix * vec4(a_vertex, 1.0);
    v_normal = a_normal;
}
