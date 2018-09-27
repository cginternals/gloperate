
#version 140
#extension GL_ARB_explicit_attrib_location : require


uniform sampler1D dofShiftKernel;
uniform sampler1D subpixelShiftKernel;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 viewport;

uniform int currentFrame;
uniform float dofZFocus;

uniform bool useAntialiasing;
uniform bool useDOF;


layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

out vec4 v_position;
out vec3 v_color;
flat out vec3 v_normal;


void main()
{
    // Get shift values
    vec2 dofShift = texelFetch(dofShiftKernel, currentFrame, 0).xy;
    vec2 subpixelShift = texelFetch(subpixelShiftKernel, currentFrame, 0).xy;
    subpixelShift /= viewport.zw;

    // Apply DOF shift in view space
    vec4 viewPos = modelViewMatrix * vec4(a_vertex, 1.0);
    vec4 dofViewPos = vec4(viewPos.xy + dofShift * (viewPos.z + dofZFocus) * float(useDOF), viewPos.zw);
    vec4 dofShiftedPos = projectionMatrix * dofViewPos;

    // Apply AA shift in screen space
    v_position = dofShiftedPos + vec4(subpixelShift, 0.0, 0.0) * float(useAntialiasing);
    v_color = vec3(0.6) + 0.2 * a_vertex + 0.2 * (a_normal * 0.5 + 0.5);
    v_normal = a_normal;
    gl_Position = v_position;
}
