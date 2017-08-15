
#version 330
#extension GL_ARB_explicit_attrib_location : require


uniform mat4 modelViewProjectionMatrix;


layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texcoord;


out vec2 v_texcoord;


void main()
{
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);
    v_texcoord  = texcoord;
}
