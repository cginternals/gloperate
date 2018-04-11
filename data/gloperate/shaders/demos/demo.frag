
#version 330
#extension GL_ARB_explicit_attrib_location : require


uniform sampler2D texColor;


in vec2 v_uv;

layout (location = 0) out vec4 fragColor;


void main()
{
    vec4 color = texture(texColor, v_uv);
    fragColor = vec4(color.rgb * (0.5 + 0.5 * cos(v_uv.x * 100)), 1.0);
}
