#version 330 core

layout(triangle) in;
layout(line_strip, max_vertices = 4) out;

void main()
{
    gl_Postion = gl_in[0].gl_Postion;
    EmitVertex();
    gl_Postion = gl_in[1].gl_Postion;
    EmitVertex();
    gl_Postion = gl_in[2].gl_Postion;
    EmitVertex();
    gl_Postion = gl_in[0].gl_Postion;
    EmitVertex();
    EndPrimitive();
}
