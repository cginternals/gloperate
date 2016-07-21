#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 viewProjection;

in vec4 v_tangent[];
in vec4 v_bitangent[];

in vec4 v_uvRect[];

in vec4 v_fontColor[];

out vec2 g_uv;
out vec4 g_fontColor;
//flat out vec3 g_normal;

void main()
{
    vec4 origin = gl_in[0].gl_Position;
    //vec3 normal = normalize(cross(normalize(v_tangent[0].xyz)
    //    , normalize(v_bitangent[0].xyz)));

    // lower right
    gl_Position = origin + v_tangent[0];
    g_uv = v_uvRect[0].zy;
    //g_normal = normal;
    gl_Position = viewProjection * gl_Position;
    g_fontColor = v_fontColor[0];

    EmitVertex();
    
    // upper right
    gl_Position = origin + v_bitangent[0] + v_tangent[0];
    g_uv = v_uvRect[0].zw;
    //g_normal = normal;
    gl_Position = viewProjection * gl_Position;
    g_fontColor = v_fontColor[0];

    EmitVertex();
    
    // lower left
    gl_Position = origin;
    g_uv = v_uvRect[0].xy;
    //g_normal = normal;
    gl_Position = viewProjection * gl_Position;
    g_fontColor = v_fontColor[0];

    EmitVertex();

    // upper left
    gl_Position = origin + v_bitangent[0];
    g_uv = v_uvRect[0].xw;
    //g_normal = normal;
    gl_Position = viewProjection * gl_Position;
    g_fontColor = v_fontColor[0];

    EmitVertex();

    EndPrimitive();
}
