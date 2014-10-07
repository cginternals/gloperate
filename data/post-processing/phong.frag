#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D geometry;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;
in vec3 v_eye;

const vec3 lpos = vec3(0.0, 2.0, 5.0);

const vec3 specular = vec3(0.6, 0.6, 0.6);
const float shininess = 128.0;
const vec3  ambient = vec3(0.08, 0.10, 0.14);
const float ambifake = 0.33;

void main()
{
	vec3 color = texture(color, v_uv).rgb;
	vec3 n = normalize(texture(normal, v_uv).rgb);
	vec3 g = texture(geometry, v_uv).rgb;
	
	vec3 l = normalize(lpos - g);
	vec3 e = normalize(v_eye);
	vec3 h = normalize(l + e);

	float ldotn = mix(ambifake, 1.0, dot(l, n));
	float hdotn = dot(h, n);

	if(ldotn > 0.0)
		fragColor = vec4(color + 
		  specular * clamp(pow(hdotn, shininess), 0.0, 1.0)
		+ ambient, 1.0);
	else
		fragColor = mix(vec4(color, 1.0), vec4(ambient, 1.0), 0.5);
}