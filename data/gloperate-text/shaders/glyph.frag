#version 330

uniform sampler2D glyphs;
uniform vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

in vec2 g_uv;
//flat in vec3 g_normal;

layout (location = 0) out vec4 out_color;

const float t = 0.5;
const int channel = 0;

void main()
{
	float distance = texture(glyphs, g_uv)[channel];
	float dFdD = fwidth(distance);

	out_color = vec4(color.rgb, color.a * smoothstep(t - dFdD, t + dFdD, distance));
}
