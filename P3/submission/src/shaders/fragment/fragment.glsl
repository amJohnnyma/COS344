#version 330 core
in vec3 vWorldPos;
out vec4 fragColor;

uniform vec4 uColor;
uniform bool wireframe;

void main()

{
	if(wireframe)
	{
fragColor = uColor;
	return;
	}
    // Reconstruct flat face normal from screen-space derivatives
    vec3 norm = normalize(cross(dFdx(vWorldPos), dFdy(vWorldPos)));

    // Fixed light slightly above and to the right of the camera
    vec3 lightDir = normalize(vec3(0.6, 1.0, 0.8));

    float ambient  = 0.25;
    float diffuse  = max(dot(norm, lightDir), 0.0);

    // Small back-face fill so rear faces aren't totally black
    float backfill = max(dot(-norm, lightDir), 0.0) * 0.1;

    float intensity = ambient + diffuse + backfill;
    fragColor = vec4(uColor.rgb * intensity, uColor.a);
}
