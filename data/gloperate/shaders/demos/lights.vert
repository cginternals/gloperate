
#version 140
#extension GL_ARB_explicit_attrib_location : require


uniform mat4 modelMatrix;
uniform mat4 modelViewProjectionMatrix;


layout (location = 0) in vec3 a_vertex;

out vec3 v_worldPosition;


void main()
{
    gl_Position = modelViewProjectionMatrix * vec4(a_vertex, 1.0);

    v_worldPosition = (modelMatrix * vec4(a_vertex, 1.0)).xyz;
}
