#version 330

uniform sampler2D glyphs;
uniform vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec2 offset = vec2(0.0, 0.0);

in vec2 g_uv;
//flat in vec3 g_normal;

layout (location = 0) out vec4 out_color;

const int channel = 0;


float aastep(float t, float value) 
{
	//float afwidth = length(vec2(dFdx(value), dFdy(value))) * 1.0;
	float afwidth = fwidth(value) * 1.0;
	return smoothstep(t - afwidth, t + afwidth, value);
}

float tex(float t, vec2 uv)
{
	return aastep(t, texture(glyphs, uv)[channel]);
}

float aastep3(float t, vec2 uv)
{
	float y = dFdy(uv.y) * 1.0 / 3.0;  

	float v = tex(t, uv + vec2( 0,-y))
          	+ tex(t, uv + vec2( 0, 0))
          	+ tex(t, uv + vec2( 0,+y));

	return v / 3.0;
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

float aastep3x3(float t, vec2 uv)
{
	float x = dFdx(uv.x) * 1.0 / 3.0;
	float y = dFdy(uv.y) * 1.0 / 3.0;  

	float v = tex(t, uv + vec2(-x,-y))
	        + tex(t, uv + vec2(-x, 0))
	        + tex(t, uv + vec2(-x,+y))

	      	+ tex(t, uv + vec2( 0,-y))
	      	+ tex(t, uv + vec2( 0, 0))
	      	+ tex(t, uv + vec2( 0,+y))

	      	+ tex(t, uv + vec2(+x,-y))
	      	+ tex(t, uv + vec2(+x, 0))
	      	+ tex(t, uv + vec2(+x,+y));

	return v / 9.0;
}

float aastep4x4s(float t, vec2 uv)
{
	float x1 = dFdx(uv.x) * 1.0 / 8.0;
	float y1 = dFdy(uv.y) * 1.0 / 8.0;
	float x2 = dFdx(uv.x) * 3.0 / 8.0;
	float y2 = dFdy(uv.y) * 3.0 / 8.0;  

	float v = tex(t, uv + vec2(-x2,-y2))    
	        + tex(t, uv + vec2(-x2,-y1))
	        + tex(t, uv + vec2(-x2,+y1))
	        + tex(t, uv + vec2(-x2,+y2))

	        + tex(t, uv + vec2(-x1,-y2))
	        + tex(t, uv + vec2(-x1,-y1))
	        + tex(t, uv + vec2(-x1,+y1))
	        + tex(t, uv + vec2(-x1,+y2))

	      	+ tex(t, uv + vec2(+x1,-y2))
	        + tex(t, uv + vec2(+x1,-y1))
	        + tex(t, uv + vec2(+x1,+y1))
	        + tex(t, uv + vec2(+x1,+y2))

	        + tex(t, uv + vec2(+x2,-y2))
	        + tex(t, uv + vec2(+x2,-y1))
	        + tex(t, uv + vec2(+x2,+y1))
	        + tex(t, uv + vec2(+x2,+y2));

	return v / 16.0;
}

vec4 subpix(float r, float g, float b, vec4 fore, vec4 back)
{
	return vec4(mix(back.rgb, fore.rgb, vec3(r, g, b)), mix(back.a, fore.a, (r + b + g) / 3.0));
}

void main()
{
	// requires blend: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float s = texture(glyphs, g_uv).r;
	if(s < 0.4)
		discard;

	float f = dFdx(g_uv.x);
	float x = sign(g_uv.x) * f / 3.0;

	vec4 fc = vec4(vec3(0.0), 1.0);
	vec4 bc = vec4(vec3(1.0), 0.0);

	// subpixel variations

//	float r, g, b;

//	if(x > 0.002) // optional: only use if glyph is small (todo: adjust threshold)
//	{	// higher quality - more samples
//		r = aastep8(0.5, g_uv + vec2(-x, 0.0));
//		g = aastep8(0.5, g_uv + vec2( 0, 0.0));
//		b = aastep8(0.5, g_uv + vec2(+x, 0.0));
//	}
//	else
//	{	// lower quality - less samples
//		r = aastep3(0.5, g_uv + vec2(-x, 0.0));
//		g = aastep3(0.5, g_uv + vec2( 0, 0.0));
//		b = aastep3(0.5, g_uv + vec2(+x, 0.0));
//	}
//	fragColor = subpix(r, g, b, fc, bc);

	// non-subpixel variations

	float a;

//	a =    aastep3(0.5, g_uv);
//	a =    aastep8(0.5, g_uv);
//	a =  aastep3x3(0.5, g_uv);
	a = aastep4x4s(0.5, g_uv);

	out_color = vec4(fc.rgb, fc.a * a);
}
