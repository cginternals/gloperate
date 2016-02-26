#version 330

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

in vec3 v_tangent[];
in vec3 v_bitangent[];
in vec2 v_glyphStart[];
in vec2 v_glyphEnd[];

out vec2 g_glyphCoordinate;
flat out vec3 g_normal;

void main()
{
    vec4 coords = gl_in[0].gl_Position;
    vec3 normal = normalize(cross(normalize(v_tangent[0]), normalize(v_bitangent[0])));
    
    // lr
    gl_Position = coords + vec4(v_tangent[0], 0.0);
    g_glyphCoordinate = vec2(v_glyphEnd[0].x, v_glyphStart[0].y);
    g_normal = normal;
    EmitVertex();
    
    // ur
    gl_Position = coords + vec4(v_tangent[0] + v_bitangent[0], 0.0);
    g_glyphCoordinate = vec2(v_glyphEnd[0].x, v_glyphEnd[0].y);
    g_normal = normal;
    EmitVertex();
    
    // ll
    gl_Position = coords;
    g_glyphCoordinate = vec2(v_glyphStart[0].x, v_glyphStart[0].y);
    g_normal = normal;
    EmitVertex();
    
    // ul
    gl_Position = coords + vec4(v_bitangent[0], 0.0);
    g_glyphCoordinate = vec2(v_glyphStart[0].x, v_glyphEnd[0].y);
    g_normal = normal;
    EmitVertex();
    
    EndPrimitive();
}
