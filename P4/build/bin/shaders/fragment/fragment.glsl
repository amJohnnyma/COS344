#version 330 core
in vec3 vWorldPos;
out vec4 fragColor;

uniform vec4 uColor;
uniform bool wireframe;
uniform vec3 uLightPos;
uniform float uLightRadius;
uniform vec3 uLightColor;

void main()
{
	if(wireframe)
	{
    fragColor = uColor;
	return;


    }
    vec3 norm = normalize(cross(dFdx(vWorldPos), dFdy(vWorldPos)));

    vec3 toLight = uLightPos - vWorldPos;
    float dist = length(toLight);
    vec3 lightDir = toLight / dist;

    float attenuation = 1.0 / (1.0 + (dist * dist) / (uLightRadius * uLightRadius));

    float ambient = 0.25;
    float diffuse = max(dot(norm, lightDir), 0.0);
    float backfill = max(dot(-norm, lightDir), 0.0) * 0.1;
    vec3 ambientContrib = uColor.rgb * ambient;
    vec3 lightContrib = uColor.rgb * (diffuse + backfill) * attenuation * uLightColor;


    fragColor = vec4(ambientContrib + lightContrib, uColor.a);


}
