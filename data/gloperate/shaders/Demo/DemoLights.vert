#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 modelMatrix;
uniform mat4 modelViewProjection;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

out vec3 v_worldPosition;
flat out vec3 v_normal;

void main()
{
    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);

    v_worldPosition = (modelMatrix * vec4(a_vertex, 1.0)).xyz;
    v_normal = normalize((modelMatrix * vec4(a_normal, 0.0)).xyz);
}
