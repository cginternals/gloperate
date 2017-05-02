#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 modelViewProjection;
uniform float z;

layout (location = 0) in vec2 a_vertex;

out vec2 v_localPos;

void main()
{
    gl_Position = modelViewProjection * vec4(a_vertex, z, 1.0);
    v_localPos = a_vertex;
}
