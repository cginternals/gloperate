
#version 140
#extension GL_ARB_explicit_attrib_location : require


layout (location = 0) in vec2 a_vertex;

uniform sampler2D gradientTexture;
uniform uint      gradientIndex;
uniform float     gradientColor;

out vec2 v_uv;
out vec4 v_color;


void main()
{
    v_uv = a_vertex * 0.5 + 0.5;
    gl_Position = vec4(a_vertex, 0.0, 1.0);

    ivec2 size = textureSize(gradientTexture, 0);
    v_color = texelFetch(gradientTexture, ivec2(int(gradientColor * float(size.x-1)), gradientIndex), 0);
}
