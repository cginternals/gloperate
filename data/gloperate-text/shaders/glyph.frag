#version 330

uniform sampler2D glyphs;

in vec2 g_glyphCoordinate;
flat in vec3 g_normal;

layout (location=0) out vec4 out_color;

void main()
{
    float distanceValue = texture(glyphs, g_glyphCoordinate).r;
    
    out_color = vec4(vec3(mix(0.0, 1.0, step(distanceValue, 0.7))), 1.0);
}
