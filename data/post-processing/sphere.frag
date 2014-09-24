#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 colorOut;
layout (location = 1) out vec4 normalOut;
layout (location = 2) out vec4 geomOut;

smooth in vec3 v_normal;
smooth in vec3 v_vertex;

void main()
{
	colorOut = vec4(0.4, 0.4, 0.3, 1.0);
	normalOut = vec4(normalize(v_normal), 1.0);
	geomOut = vec4(v_vertex, 1.0);
}
