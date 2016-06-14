#version 330
#extension GL_ARB_explicit_attrib_location : require


layout (location = 0) in vec2 vertex;


out vec2 v_uv;


void main()
{
    v_uv = vec2(vertex.s, 1.0 - vertex.t);
    gl_Position = vec4(vertex, 0.0, 1.0);
}
