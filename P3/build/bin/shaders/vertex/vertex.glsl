#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vec4 viewPos = uView * uModel * vec4(aPos, 1.0);
    FragPos      = viewPos.xyz;
    gl_Position  = uProj * viewPos;
}
