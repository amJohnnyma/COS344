#version 330 core
in vec3 vWorldPos;
out vec4 fragColor;

uniform vec4 uColor;
uniform bool wireframe;
uniform int uTextureMask;

uniform sampler2D uTexture;       // unit 0 - checker
uniform sampler2D uDimpleColor;   // unit 1
uniform sampler2D uDisplacement;  // unit 2
uniform sampler2D uAlpha;         // unit 3

uniform vec3 uLightPos;
uniform float uLightRadius;
uniform vec3 uLightColor;
uniform vec3 uSphereCenter;

void main()
{
    if(wireframe) {
        fragColor = uColor;
        return;
    }

vec3 localDir = normalize(vWorldPos - uSphereCenter);
float u = 0.5 + atan(localDir.z, localDir.x) / (2.0 * 3.14159265);
float v = 0.5 - asin(clamp(localDir.y, -1.0, 1.0)) / 3.14159265;
vec2 uv = vec2(u, v) * 10.0;
    vec3 norm = normalize(cross(dFdx(vWorldPos), dFdy(vWorldPos)));
    vec3 toLight = uLightPos - vWorldPos;
    float dist = length(toLight);
    float attenuation = 1.0 / (1.0 + (dist * dist) / (uLightRadius * uLightRadius));
    float ambient = 0.25;
    float diffuse = max(dot(norm, normalize(toLight)), 0.0);

    // Base color
    vec3 baseColor = uColor.rgb;
    float alpha = uColor.a;

    if ((uTextureMask & 1) != 0)
        baseColor = texture(uTexture, uv).rgb;

    if ((uTextureMask & 2) != 0)
        baseColor = mix(baseColor, texture(uDimpleColor, uv).rgb, 1.0);

    if ((uTextureMask & 4) != 0) {
        // Displacement visual only — actual geo displacement needs vertex shader
        float disp = texture(uDisplacement, uv).r;
        baseColor *= (0.7 + 0.3 * disp); // fake darkening where displaced
    }

    if ((uTextureMask & 8) != 0)
        alpha *= texture(uAlpha, uv).r;

    vec3 finalRGB = baseColor * (diffuse + ambient) * attenuation * uLightColor;
    fragColor = vec4(finalRGB, alpha);

}
