
#version 330


//uniform mat4 viewProjectionMatrix;


layout (location = 0) in vec3 in_origin;
layout (location = 1) in vec3 in_vtan;
layout (location = 2) in vec3 in_vbitan;
layout (location = 3) in vec4 in_uvRect;  // vec2 lowerLeft and vec2 upperRight in glyph texture (uv)
layout (location = 4) in vec4 in_fontColor;

out vec4 v_tangent;
out vec4 v_bitangent;
out vec4 v_uvRect;
out vec4 v_fontColor;


void main()
{
	//vec4 position = viewProjectionMatrix * vec4(in_origin, 1.0);
    //position /= position.w;
    //gl_Position = position;
    gl_Position = vec4(in_origin, 1.0);

    v_tangent   = vec4(in_vtan,   0.0);
    v_bitangent = vec4(in_vbitan, 0.0);
    v_uvRect    = in_uvRect;
    v_fontColor = in_fontColor;
}
