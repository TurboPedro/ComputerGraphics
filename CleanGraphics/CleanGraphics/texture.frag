#version 430

#define NR_POINT_LIGHTS 4

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

out vec4 FragColor;

void main()
{

	vec3 finalColor = vec3(0.0);
	// view vector : vec3(0, 0, 0) is camera position
	vec3 V = normalize(vec3(0, 0, 0) - Position.xyz);

	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
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

}