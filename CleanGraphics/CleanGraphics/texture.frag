#version 430

#define NR_POINT_LIGHTS 5

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 La;
	vec3 Ld;
	vec3 Ls;

	float Constant;
	float Linear;
	float Quadratic;
};
uniform LightInfo Lights[NR_POINT_LIGHTS];

struct MaterialInfo {
	float Alpha;
	float Shiness;      // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;
uniform int LightsNumber;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main()
{

	vec3 finalColor = vec3(0.0);
	// view vector : vec3(0, 0, 0) is camera position
	vec3 V = normalize(vec3(0, 0, 0) - Position.xyz);

	for (int i = 0; i < LightsNumber; i++) {
		// light vector
		vec3 L = normalize(Lights[i].Position.xyz - Position);

		// reflection vector
		vec3 H = normalize(V + L);
		vec3 R = normalize(reflect(-L, Normal));

		float distance = length(Lights[i].Position.xyz - Position);
		float attenuation = 1.0 / (Lights[i].Constant + Lights[i].Linear * distance + Lights[i].Quadratic * (distance * distance));

		vec3 ambient = vec3(texture(DiffuseTexture, TexCoord)) * Lights[i].La;
		vec3 diffuse = vec3(texture(DiffuseTexture, TexCoord)) * Lights[i].Ld * max(dot(L, Normal), 0);
		vec3 specular = Lights[i].Ls * pow(max(dot(H, Normal), 0.0), Material.Shiness) * vec3(texture(SpecularTexture, TexCoord));

		finalColor += (ambient + diffuse + specular) * attenuation;
	}

	FragColor = vec4(finalColor, Material.Alpha);

	float brightness = dot(FragColor.rgb, vec3(0.02126, 0.7152, 0.0722));
	if (brightness > 1.0)
		BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}