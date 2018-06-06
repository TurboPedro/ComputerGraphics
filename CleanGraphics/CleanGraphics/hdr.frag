#version 430 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D SceneTex;
uniform sampler2D BloomTex;
uniform float Gamma;
uniform float Exposure;

void main()
{
	vec3 hdrColor = texture(SceneTex, TexCoords).rgb;
    vec3 bloomColor = texture(BloomTex, TexCoords).rgb;
    hdrColor += bloomColor;

	// Exposure tone mapping
	vec3 mapped = vec3(1.0) - exp(-hdrColor * Exposure);

	// Gamma correction
	mapped = pow(mapped, vec3(1.0 / Gamma));

    FragColor = vec4(mapped, 1.0);
}