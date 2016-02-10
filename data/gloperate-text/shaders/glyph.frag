#version 330

uniform sampler2D glyphs;

in vec2 g_uv;
flat in vec3 g_normal;

layout (location = 0) out vec4 out_color;


const vec4 fc = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 bc = vec4(1.0, 1.0, 1.0, 0.0);
const float t = 0.5;


float aastep(float t, float value) 
{
	float afwidth = fwidth(value);
	return smoothstep(t - afwidth, t + afwidth, value);
}

float tex(float t, vec2 uv)
{
	return aastep(t, texture(glyphs, uv).r);
}

float aastep3(float t, vec2 uv, float dx, float dy)
{
	float y = dy / 3.0;  

	float v = tex(t, uv + vec2( 0,-y))
            + tex(t, uv + vec2( 0, 0))
            + tex(t, uv + vec2( 0,+y));

	return v / 3.0;
}

float aastep3(float t, vec2 uv)
{
	float y = dFdy(uv.y) * 1.0 / 3.0;  

	float v = tex(t, uv + vec2( 0,-y))
            + tex(t, uv + vec2( 0, 0))
            + tex(t, uv + vec2( 0,+y));

	return v / 3.0;
}

float aastep8(float t, vec2 uv, float dx, float dy)
{
	float x1 = dx * 1.0 / 8.0;
	float y1 = dy * 1.0 / 8.0;  
	float y2 = dy * 3.0 / 8.0;  

	float v = tex(t, uv + vec2(-x1,-y2))
            + tex(t, uv + vec2(-x1,-y1))
            + tex(t, uv + vec2(-x1,+y1))
            + tex(t, uv + vec2(-x1,+y2))
            + tex(t, uv + vec2(+x1,-y2))
            + tex(t, uv + vec2(+x1,-y1))
            + tex(t, uv + vec2(+x1,+y1))
            + tex(t, uv + vec2(+x1,+y2));

	return v / 8.0;
}

float aastep8(float t, vec2 uv)
{
	float x1 = dFdx(uv.x) * 1.0 / 8.0;
	float y1 = dFdy(uv.y) * 1.0 / 8.0;  
	float y2 = dFdy(uv.y) * 3.0 / 8.0;  

	float v = tex(t, uv + vec2(-x1,-y2))
            + tex(t, uv + vec2(-x1,-y1))
            + tex(t, uv + vec2(-x1,+y1))
            + tex(t, uv + vec2(-x1,+y2))
            + tex(t, uv + vec2(+x1,-y2))
            + tex(t, uv + vec2(+x1,-y1))
            + tex(t, uv + vec2(+x1,+y1))
            + tex(t, uv + vec2(+x1,+y2));

	return v / 8.0;
}

vec4 subpix(float r, float g, float b, vec4 fore, vec4 back)
{
	return vec4(mix(back.rgb, fore.rgb, vec3(r, g, b)), mix(back.a, fore.a, (r + g + b) / 3.0));
}

vec4 subpix_ext(float r, float g, float b, vec4 fc)
{
	// https://github.com/rougier/freetype-gl/blob/master/shaders/text.frag

	float t0 = min(min(r, g), b);
	float t1 = max(max(r, g), b);

	vec4 color = vec4(fc.rgb, (r + g + b) / 3.0);
	color = mix(vec4(r, g, b, t0), color, vec4(t1));

	return vec4(color.rgb, fc.a * color.a);
}

vec4 fast_pass(float x)
{
	float g0 = tex(t, g_uv + vec2(-3.0 * x, 0.0));
	float b0 = tex(t, g_uv + vec2(-2.0 * x, 0.0));
	float r1 = tex(t, g_uv + vec2(-1.0 * x, 0.0));
	float g1 = tex(t, g_uv + vec2( 0.0 * x, 0.0));
	float b1 = tex(t, g_uv + vec2(+1.0 * x, 0.0));
	float r2 = tex(t, g_uv + vec2(+2.0 * x, 0.0));
	float g2 = tex(t, g_uv + vec2(+3.0 * x, 0.0));

	float r = 3.0 * r1 + g0 + 2.0 * (b0 + g1) + b1;
	float g = 3.0 * g1 + b0 + 2.0 * (r1 + b1) + r2;
	float b = 3.0 * b1 + r1 + 2.0 * (g1 + r2) + g2;

	r /= 9.0;
	g /= 9.0;
	b /= 9.0;

	return subpix(r, g, b, fc, bc); // * vec4(0.5, 1.0, 0.5, 1.0);
}

vec4 small_pass(float x, float dx, float dy) // use for 2d transformed glyphs (e.g., screen space)
{
	float g0 = aastep3(t, g_uv + vec2(-3.0 * x, 0.0), dx, dy);
	float b0 = aastep3(t, g_uv + vec2(-2.0 * x, 0.0), dx, dy);
	float r1 = aastep3(t, g_uv + vec2(-1.0 * x, 0.0), dx, dy);
	float g1 = aastep3(t, g_uv + vec2( 0.0 * x, 0.0), dx, dy);
	float b1 = aastep3(t, g_uv + vec2(+1.0 * x, 0.0), dx, dy);
	float r2 = aastep3(t, g_uv + vec2(+2.0 * x, 0.0), dx, dy);
	float g2 = aastep3(t, g_uv + vec2(+3.0 * x, 0.0), dx, dy);

	float r = 3.0 * r1 + g0 + 2.0 * (b0 + g1) + b1;
	float g = 3.0 * g1 + b0 + 2.0 * (r1 + b1) + r2;
	float b = 3.0 * b1 + r1 + 2.0 * (g1 + r2) + g2;

	r /= 9.0;
	g /= 9.0;
	b /= 9.0;

	return subpix_ext(r, g, b, fc); // * vec4(1.0, 0.5, 0.5, 1.0);	
}

vec4 small_pass(float x) // use for 3d transformed glyphs (e.g., world space)
{
	float g0 = aastep3(t, g_uv + vec2(-3.0 * x, 0.0));
	float b0 = aastep3(t, g_uv + vec2(-2.0 * x, 0.0));
	float r1 = aastep3(t, g_uv + vec2(-1.0 * x, 0.0));
	float g1 = aastep3(t, g_uv + vec2( 0.0 * x, 0.0));
	float b1 = aastep3(t, g_uv + vec2(+1.0 * x, 0.0));
	float r2 = aastep3(t, g_uv + vec2(+2.0 * x, 0.0));
	float g2 = aastep3(t, g_uv + vec2(+3.0 * x, 0.0));

	float r = 3.0 * r1 + g0 + 2.0 * (b0 + g1) + b1;
	float g = 3.0 * g1 + b0 + 2.0 * (r1 + b1) + r2;
	float b = 3.0 * b1 + r1 + 2.0 * (g1 + r2) + g2;

	r /= 9.0;
	g /= 9.0;
	b /= 9.0;

	return subpix_ext(r, g, b, fc); // * vec4(1.0, 0.5, 0.5, 1.0);	
}

void main()
{
	// https://www.grc.com/ctwhat.htm
	// requires blend: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float dx = dFdx(g_uv.x);
	float dy = dFdy(g_uv.y);

	float x = sign(g_uv.x) * dx / 3.0;

	// the threshold was derived empirically (needs to be adjusted 
	// based on the font face's font size ... this value works for 72pt)
	out_color = x > 0.002 ? small_pass(x, dx, dy) : fast_pass(x);
}
	