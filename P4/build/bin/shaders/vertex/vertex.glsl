#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform sampler2D uDisplacement;
uniform int uTextureMask;
uniform float uDisplaceStrength;

out vec3 vWorldPos;
out vec2 vTexCoord;

void main()
{
    vec3 pos = aPos;

    if ((uTextureMask & 4) != 0) {
        float disp = texture(uDisplacement, aTexCoord).r;
        pos += aNormal * (disp - 1.0) * uDisplaceStrength; // -1 pushes inward
    }

    vec4 worldPos = uModel * vec4(pos, 1.0);
    vWorldPos = worldPos.xyz;
    vTexCoord = aTexCoord;
    gl_Position = uProj * uView * worldPos;
}
