
#version 140
#extension GL_ARB_explicit_attrib_location : require


uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec2 dofShift;
uniform vec2 subpixelShift;


layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

out vec4 v_position;
flat out vec3 v_normal;


void main()
{
    vec4 viewPos = viewMatrix * vec4(a_vertex, 1.0);
    vec4 dofViewPos = vec4(viewPos.xy + dofShift * (viewPos.z + 0.1), viewPos.zw);
    vec4 dofShiftedPos = projectionMatrix * dofViewPos;

    v_position = dofShiftedPos + vec4(subpixelShift, 0.0, 0.0);
    v_normal = a_normal;
    gl_Position = v_position;
}
