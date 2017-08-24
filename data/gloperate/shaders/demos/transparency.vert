
#version 140
#extension GL_ARB_explicit_attrib_location : require


layout (location = 0) in vec2 a_vertex;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec4 a_color;

flat out vec4 v_color;
out vec2 v_localPos;


void main()
{
    gl_Position = vec4(a_vertex, 0.0, 1.0);
    v_color = a_color;
    v_localPos = a_uv;
}
