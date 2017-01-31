#version 330
#extension GL_ARB_explicit_attrib_location : require


layout (location = 0) in vec2 vertex;


out vec2 v_uv;


void main()
{
    v_uv = vertex * 0.5 + 0.5;
    gl_Position = vec4(vertex, 0.0, 1.0);
}
