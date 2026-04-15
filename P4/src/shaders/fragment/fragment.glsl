#version 330 core
in vec3 vWorldPos;
out vec4 fragColor;

uniform vec4 uColor;
uniform bool wireframe;
uniform sampler2D uTexture; 
uniform bool uUseTexture;   

uniform vec3 uLightPos;
uniform float uLightRadius;
uniform vec3 uLightColor;

void main()
{
    if(wireframe) {
        fragColor = uColor;
        return;

    }

    // World-space Planar Mapping (X and Z planes)
    // 0.5 scale makes the grid squares larger
    vec2 uv = vWorldPos.xz * 0.5;
    vec4 texColor = texture(uTexture, uv);

    vec3 norm = normalize(cross(dFdx(vWorldPos), dFdy(vWorldPos)));
    vec3 toLight = uLightPos - vWorldPos;
    float dist = length(toLight);
    float attenuation = 1.0 / (1.0 + (dist * dist) / (uLightRadius * uLightRadius));
    
    float ambient = 0.25;
    float diffuse = max(dot(norm, normalize(toLight)), 0.0);
    
    // Mix the checkerboard with your object color
    vec3 baseColor = uUseTexture ? texColor.rgb : uColor.rgb;
    vec3 finalRGB = baseColor * (diffuse + ambient) * attenuation * uLightColor;

    fragColor = vec4(finalRGB, uColor.a);
}
