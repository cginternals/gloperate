
#version 140
#extension GL_ARB_explicit_attrib_location : require


uniform mat4      modelViewProjectionMatrix;
uniform sampler2D gradientTexture;
uniform uint      gradientIndex;
uniform float     value;


layout (location = 0) in vec3 vertex;


out vec4 v_color;


void main()
{
    gl_Position = modelViewProjectionMatrix * vec4(vertex, 1.0);

    ivec2 size = textureSize(gradientTexture, 0);
    v_color = texelFetch(gradientTexture, ivec2(int(value * float(size.x-1)), gradientIndex), 0);
}
