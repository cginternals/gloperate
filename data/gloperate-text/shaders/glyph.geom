#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 v_tangent[];
in vec3 v_bitangent[];

in vec4 v_uvRect[];

out vec2 g_uv;
flat out vec3 g_normal;

void main()
{
    vec4 coords = gl_in[0].gl_Position;
    vec3 normal = normalize(cross(normalize(v_tangent[0]), normalize(v_bitangent[0])));
    
    // lower left
    gl_Position = coords;
    g_uv = v_uvRect[0].xy;
    g_normal = normal;
    EmitVertex();

    // upper left
    gl_Position = coords + vec4(v_bitangent[0], 0.0);
    g_uv = v_uvRect[0].xw;
    g_normal = normal;
    EmitVertex();
    
    // lower right
    gl_Position = coords + vec4(v_tangent[0], 0.0);
    g_uv = v_uvRect[0].zy;
    g_normal = normal;
    EmitVertex();
    
    // upper right
    gl_Position = coords + vec4(v_bitangent[0] + v_tangent[0], 0.0);
    g_uv = v_uvRect[0].zw;
    g_normal = normal;
    EmitVertex();

    EndPrimitive();
}
