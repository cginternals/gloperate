
#version 140
#extension GL_ARB_explicit_attrib_location : require


uniform vec2 offset;


layout (location = 0) in vec2 a_vertex;

out vec2 v_uv;


void main()
{
    gl_Position = vec4(a_vertex + offset, 0.0, 1.0);
}
