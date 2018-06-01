#version 430 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D ScreenTex;
uniform float Gamma;

void main()
{
	vec3 hdrColor = texture(ScreenTex, TexCoords).rgb;

	// Reinhard tone mapping
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

	//gamma correction
	mapped = pow(mapped, vec3(1.0 / Gamma));

    FragColor = vec4(mapped, 1.0);
}