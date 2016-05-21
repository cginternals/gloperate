#version 330
#extension GL_ARB_explicit_attrib_location : require


uniform sampler2D texColor;
uniform sampler2D texColor2;
uniform float     factor = 0.5;


in vec2 v_uv;


layout (location = 0) out vec4 fragColor;


void main()
{
    vec4 color1 = texture(texColor,  v_uv);
    vec4 color2 = texture(texColor2, v_uv);
    fragColor = mix(color1, color2, abs(mod(factor, 2.0) - 1.0));
}
