
#version 330
#extension GL_ARB_explicit_attrib_location : require


uniform sampler2D tex0;


in vec2 v_texcoord;


layout (location = 0) out vec4 colorOut;


void main()
{
    // Output color
    colorOut = texture(tex0, v_texcoord);
}
