
#version 140
#extension GL_ARB_explicit_attrib_location : require


uniform sampler1D dofShiftKernel;
uniform sampler1D subpixelShiftKernel;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 viewport;

uniform int currentFrame;
uniform float dofZFocus = 0.1;

uniform bool useAntialiasing;
uniform bool useDOF;


layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

out vec4 v_position;
flat out vec3 v_normal;


void main()
{
    vec2 dofShift = texelFetch(dofShiftKernel, currentFrame, 0).xy;
    vec2 subpixelShift = texelFetch(subpixelShiftKernel, currentFrame, 0).xy;
    subpixelShift /= viewport.zw;

    vec4 viewPos = viewMatrix * vec4(a_vertex, 1.0);
    vec4 dofViewPos = vec4(viewPos.xy + dofShift * (viewPos.z + dofZFocus) * float(useDOF), viewPos.zw);
    vec4 dofShiftedPos = projectionMatrix * dofViewPos;

    v_position = dofShiftedPos + vec4(subpixelShift, 0.0, 0.0) * float(useAntialiasing);
    v_normal = a_normal;
    gl_Position = v_position;
}
