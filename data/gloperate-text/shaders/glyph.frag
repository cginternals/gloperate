#version 330

uniform sampler2D glyphs;

in vec2 g_uv;
flat in vec3 g_normal;

layout (location = 0) out vec4 out_color;

void main()
{
    float d = texture(glyphs, vec2(g_uv.x, g_uv.y)).r;

    if(d < 0.5)
    	discard;

    out_color = vec4(1.0);
}
