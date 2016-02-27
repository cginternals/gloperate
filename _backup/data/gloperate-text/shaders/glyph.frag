#version 330

uniform sampler2D glyphs;
uniform vec4 fontColor = vec4(1.0);
uniform float distanceThreshold = 0.6;

in vec2 g_glyphCoordinate;
flat in vec3 g_normal;

layout (location=0) out vec4 out_color;

void main()
{
    float distanceValue = texture(glyphs, g_glyphCoordinate).r;
    
    if (step(distanceValue, distanceThreshold) > 0.0)
    {
        out_color = fontColor;
    }
    else
    {
        discard;
    }
}
