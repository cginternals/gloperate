#version 330

uniform sampler2D glyphs;
uniform int quality = 0;

in vec2 g_uv;
flat in vec3 g_normal;

layout (location = 0) out vec4 out_color;


const vec4  fc = vec4(0.0, 0.0, 0.0, 1.0);
const vec4  bc = vec4(1.0, 1.0, 1.0, 0.0);
const vec4  ws = vec4(3.0 / 9.0, 2.0 / 9.0, 1.0 / 9.0, 0.0);

const float t = 0.5;
const int channel = 0;


//float aastep(float t, float value) 
//{
//	float afwidth = fwidth(value);
//	return smoothstep(t - afwidth, t + afwidth, value);
//}
//
//float tex(float t, vec2 uvOffset)
//{
//	return aastep(t, texture(glyphs, g_uv + uvOffset).r);
//}
//
//float aastep3(float t, vec2 uv, float dx, float dy)
//{
//	float y = dy / 3.0;  
//
//	float v = tex(t, uv + vec2( 0,-y))
//            + tex(t, uv + vec2( 0, 0))
//            + tex(t, uv + vec2( 0,+y));
//
//	return v / 3.0;
//}
//
//float aastep3(float t, vec2 uv)
//{
//	float y = dFdy(uv.y) * 1.0 / 3.0;  
//
//	float v = tex(t, uv + vec2( 0,-y))
//            + tex(t, uv + vec2( 0, 0))
//            + tex(t, uv + vec2( 0,+y));
//
//	return v / 3.0;
//}
//
//float aastep8(float t, vec2 uv, float dx, float dy)
//{
//	float x1 = dx * 1.0 / 8.0;
//	float y1 = dy * 1.0 / 8.0;  
//	float y2 = dy * 3.0 / 8.0;  
//
//	float v = tex(t, uv + vec2(-x1,-y2))
//            + tex(t, uv + vec2(-x1,-y1))
//            + tex(t, uv + vec2(-x1,+y1))
//            + tex(t, uv + vec2(-x1,+y2))
//            + tex(t, uv + vec2(+x1,-y2))
//            + tex(t, uv + vec2(+x1,-y1))
//            + tex(t, uv + vec2(+x1,+y1))
//            + tex(t, uv + vec2(+x1,+y2));
//
//	return v / 8.0;
//}
//
//float aastep8(float t, vec2 uv)
//{
//	float x1 = dFdx(uv.x) * 1.0 / 8.0;
//	float y1 = dFdy(uv.y) * 1.0 / 8.0;  
//	float y2 = dFdy(uv.y) * 3.0 / 8.0;  
//
//	float v = tex(t, uv + vec2(-x1,-y2))
//            + tex(t, uv + vec2(-x1,-y1))
//            + tex(t, uv + vec2(-x1,+y1))
//            + tex(t, uv + vec2(-x1,+y2))
//            + tex(t, uv + vec2(+x1,-y2))
//            + tex(t, uv + vec2(+x1,-y1))
//            + tex(t, uv + vec2(+x1,+y1))
//            + tex(t, uv + vec2(+x1,+y2));
//
//	return v / 8.0;
//}
//
//vec4 subpix(float r, float g, float b, vec4 fore, vec4 back)
//{
//	return vec4(mix(back.rgb, fore.rgb, vec3(r, g, b)), mix(back.a, fore.a, (r + g + b) / 3.0));
//}
//
vec4 subpix_ext(float r, float g, float b, vec4 fc)
{
	// https://github.com/rougier/freetype-gl/blob/master/shaders/text.frag

	float t0 = min(min(r, g), b);
	float t1 = max(max(r, g), b);

	vec4 color = vec4(fc.rgb, (r + g + b) / 3.0);
	color = mix(vec4(r, g, b, t0), color, vec4(t1));

	return vec4(color.rgb, fc.a * color.a);
}
//
//vec4 fast_pass(float x)
//{
//	float g0 = tex(t, g_uv + vec2(-3.0 * x, 0.0));
//	float b0 = tex(t, g_uv + vec2(-2.0 * x, 0.0));
//	float r1 = tex(t, g_uv + vec2(-1.0 * x, 0.0));
//	float g1 = tex(t, g_uv + vec2( 0.0 * x, 0.0));
//	float b1 = tex(t, g_uv + vec2(+1.0 * x, 0.0));
//	float r2 = tex(t, g_uv + vec2(+2.0 * x, 0.0));
//	float g2 = tex(t, g_uv + vec2(+3.0 * x, 0.0));
//
//	float r = 3.0 * r1 + g0 + 2.0 * (b0 + g1) + b1;
//	float g = 3.0 * g1 + b0 + 2.0 * (r1 + b1) + r2;
//	float b = 3.0 * b1 + r1 + 2.0 * (g1 + r2) + g2;
//
//	r /= 9.0;
//	g /= 9.0;
//	b /= 9.0;
//
//	return subpix(r, g, b, fc, bc); // * vec4(0.5, 1.0, 0.5, 1.0);
//}
//
//vec4 small_pass(float x, float dx, float dy) // use for 2d transformed glyphs (e.g., screen space)
//{
//	float g0 = aastep3(t, g_uv + vec2(-3.0 * x, 0.0), dx, dy);
//	float b0 = aastep3(t, g_uv + vec2(-2.0 * x, 0.0), dx, dy);
//	float r1 = aastep3(t, g_uv + vec2(-1.0 * x, 0.0), dx, dy);
//	float g1 = aastep3(t, g_uv + vec2( 0.0 * x, 0.0), dx, dy);
//	float b1 = aastep3(t, g_uv + vec2(+1.0 * x, 0.0), dx, dy);
//	float r2 = aastep3(t, g_uv + vec2(+2.0 * x, 0.0), dx, dy);
//	float g2 = aastep3(t, g_uv + vec2(+3.0 * x, 0.0), dx, dy);
//
//	float r = 3.0 * r1 + g0 + 2.0 * (b0 + g1) + b1;
//	float g = 3.0 * g1 + b0 + 2.0 * (r1 + b1) + r2;
//	float b = 3.0 * b1 + r1 + 2.0 * (g1 + r2) + g2;
//
//	r /= 9.0;
//	g /= 9.0;
//	b /= 9.0;
//
//	return subpix_ext(r, g, b, fc); // * vec4(1.0, 0.5, 0.5, 1.0);	
//}
//
//vec4 small_pass(float x) // use for 3d transformed glyphs (e.g., world space)
//{
//	float g0 = tex(t, g_uv + vec2(-3.0 * x, 0.0));
//	float b0 = tex(t, g_uv + vec2(-2.0 * x, 0.0));
//	float r1 = tex(t, g_uv + vec2(-1.0 * x, 0.0));
//	float g1 = tex(t, g_uv + vec2( 0.0 * x, 0.0));
//	float b1 = tex(t, g_uv + vec2(+1.0 * x, 0.0));
//	float r2 = tex(t, g_uv + vec2(+2.0 * x, 0.0));
//	float g2 = tex(t, g_uv + vec2(+3.0 * x, 0.0));
//
//	float r = 3.0 * r1 + g0 + 2.0 * (b0 + g1) + b1;
//	float g = 3.0 * g1 + b0 + 2.0 * (r1 + b1) + r2;
//	float b = 3.0 * b1 + r1 + 2.0 * (g1 + r2) + g2;
//
//	r /= 9.0;
//	g /= 9.0;
//	b /= 9.0;
//
//	return subpix_ext(r, g, b, fc); // * vec4(1.0, 0.5, 0.5, 1.0);	
//}

vec4 sample1(in vec4 color, in sampler2D glyphs, in vec2 uv)
{
	float distance = texture(glyphs, uv)[channel];
	float dFdD = fwidth(distance);

	color.a *= smoothstep(t - dFdD, t + dFdD, distance);
	return color;
}

vec4 sample7_subpixel(in vec4 color, in sampler2D glyphs, in vec2 uv)
{
	float dx = dFdx(uv.x);
//	float dy = dFdy(g_uv.y);
//
	float x = sign(uv.x) * dx / 3.0;
//	float y = sign(g_uv.y) * dy / 4.0;

	vec4 r1g1b100;
	r1g1b100.r = texture(glyphs, uv + vec2(-1.0 * x, 0.0))[channel];
	r1g1b100.g = texture(glyphs, uv + vec2( 0.0 * x, 0.0))[channel];
	r1g1b100.b = texture(glyphs, uv + vec2(+1.0 * x, 0.0))[channel];

	vec4 g0b0r2g2;
	g0b0r2g2.r = texture(glyphs, uv + vec2(-3.0 * x, 0.0))[channel];
	g0b0r2g2.g = texture(glyphs, uv + vec2(-2.0 * x, 0.0))[channel];
	g0b0r2g2.b = texture(glyphs, uv + vec2(+2.0 * x, 0.0))[channel];
	g0b0r2g2.a = texture(glyphs, uv + vec2(+3.0 * x, 0.0))[channel];

	vec4 fw0 = fwidth(r1g1b100);
	vec4 fw1 = fwidth(g0b0r2g2);

	r1g1b100 = smoothstep(vec4(t) - fw0, vec4(t) + fw0, r1g1b100);
	g0b0r2g2 = smoothstep(vec4(t) - fw1, vec4(t) + fw1, g0b0r2g2);

	vec3 e;
	e.r = dot(r1g1b100, ws.xyzw) + dot(g0b0r2g2, ws.zyww);
	e.g = dot(r1g1b100, ws.yxyw) + dot(g0b0r2g2, ws.wzzw);
	e.b = dot(r1g1b100, ws.zyxw) + dot(g0b0r2g2, ws.wwyz);

	//return vec4(e, 1.0);

	//return vec4(mix(bc.rgb, fc.rgb, e.rgb), mix(bc.a, fc.a, (e.r + e.g + e.b) / 3.0));
	//return r1g1b100;
	return subpix_ext(e.r, e.g, e.b, fc); // * vec4(1.0, 0.5, 0.5, 1.0);
}

void main()
{
	switch(quality)
    {
    case 0:
    	out_color = sample1(fc, glyphs, g_uv);
    	break;
    case 1:
    	out_color = sample7_subpixel(fc, glyphs, g_uv);
    	break;
    default:
    	discard;
    	break;
    }
}

//void main()
//{
//	// https://www.grc.com/ctwhat.htm
//	// requires blend: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	float dx = dFdx(g_uv.x);
//	float dy = dFdy(g_uv.y);
//
//	float x = sign(g_uv.x) * dx / 3.0;
//	float y = sign(g_uv.y) * dy / 4.0;
//
//	// the threshold was derived empirically (needs to be adjusted 
//	// based on the font face's font size ... this value works for 72pt)
//	//out_color = fast_pass(x);
//
//	vec4 r1g1b100;
//	r1g1b100.r = texture(glyphs, g_uv + vec2(-1.0 * x,-y)).r * 0.25
//			   + texture(glyphs, g_uv + vec2(-1.0 * x, 0)).r * 0.50
//			   + texture(glyphs, g_uv + vec2(-1.0 * x,+y)).r * 0.25;
//	r1g1b100.g = texture(glyphs, g_uv + vec2( 0.0 * x,-y)).r * 0.25
//			   + texture(glyphs, g_uv + vec2( 0.0 * x, 0)).r * 0.50
//			   + texture(glyphs, g_uv + vec2( 0.0 * x,+y)).r * 0.25;
//	r1g1b100.b = texture(glyphs, g_uv + vec2(+1.0 * x,-y)).r * 0.25
//			   + texture(glyphs, g_uv + vec2(+1.0 * x, 0)).r * 0.50
//			   + texture(glyphs, g_uv + vec2(+1.0 * x,+y)).r * 0.25;
//	//r1g1b100 /= 3.0;
//
//	vec4 g0b0r2g2;
//	g0b0r2g2.r = texture(glyphs, g_uv + vec2(-3.0 * x, 0)).r;
//	g0b0r2g2.g = texture(glyphs, g_uv + vec2(-2.0 * x, 0)).r;
//	g0b0r2g2.b = texture(glyphs, g_uv + vec2(+2.0 * x, 0)).r;
//	g0b0r2g2.a = texture(glyphs, g_uv + vec2(+3.0 * x, 0)).r;
//
//	vec4 fw0 = fwidth(r1g1b100);
//	vec4 fw1 = fwidth(g0b0r2g2);
//
//	r1g1b100 = smoothstep(vec4(t) - fw0, vec4(t) + fw0, r1g1b100);
//	g0b0r2g2 = smoothstep(vec4(t) - fw1, vec4(t) + fw1, g0b0r2g2);
//
//	vec3 e;
//	e.r = dot(r1g1b100, ws.xyzw) + dot(g0b0r2g2, ws.zyww);
//	e.g = dot(r1g1b100, ws.yxyw) + dot(g0b0r2g2, ws.wzzw);
//	e.b = dot(r1g1b100, ws.zyxw) + dot(g0b0r2g2, ws.wwyz);
//
//	out_color = vec4(mix(bc.rgb, fc.rgb, e.rgb), mix(bc.a, fc.a, (e.r + e.g + e.b) / 3.0));
//	//out_color =  subpix_ext(e.r, e.g, e.b, fc); // * vec4(1.0, 0.5, 0.5, 1.0);	
//}
	
// befor optimization : subpixel  = true;
// average:  2230.85µs per frame (1000 frames)
//    last:  2219.85µs
// average:  2203.63µs per frame (1000 frames)
//    last:  2211.16µs
// average:  2256.46µs per frame (1000 frames)
//   last:   2202.86µs

// befor optimization : subpixel  = false;
// average:  6281.04µs per frame (1000 frames)
//    last:  6259.75µs

