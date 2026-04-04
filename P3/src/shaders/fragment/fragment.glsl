#version 330 core
out vec4 fragColor;

in vec3 FragPos;

uniform vec4 uColor;

void main()
{
    vec3 norm     = normalize(cross(dFdx(FragPos), dFdy(FragPos)));
    vec3 lightDir = normalize(-FragPos);
    float diff    = max(dot(norm, lightDir), 0.0);

    float ambient = 0.15;
    float dist    = length(FragPos);
    float atten   = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

    float intensity = ambient + diff * atten;
    fragColor = vec4(uColor.rgb * intensity, uColor.a);
}
