#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 v_worldPosition[];

out vec3 g_worldPosition;
out vec3 g_normal;

void main()
{
    vec3 a = v_worldPosition[0];
    vec3 b = v_worldPosition[1];
    vec3 c = v_worldPosition[2];

    vec3 tangent = b - a;
    vec3 bitangent = c - a;
    vec3 normal = normalize(cross(tangent, bitangent));

    gl_Position = gl_in[0].gl_Position;
    g_worldPosition = v_worldPosition[0];
    g_normal = normal;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    g_worldPosition = v_worldPosition[1];
    g_normal = normal;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    g_worldPosition = v_worldPosition[2];
    g_normal = normal;
    EmitVertex();

    EndPrimitive();
}
