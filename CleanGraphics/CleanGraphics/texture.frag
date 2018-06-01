#version 430

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
uniform LightInfo Light;

struct MaterialInfo {
	float Alpha;
	float Shiness;      // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

out vec4 FragColor;

void main()
{

	// light vector
	vec3 L = normalize(Light.Position.xyz - Position);

	// view vector : vec3(0, 0, 0) is camera position
	vec3 V = normalize(vec3(0, 0, 0) - Position.xyz);

	// reflection vector
	vec3 H = normalize(V + L);
	vec3 R = normalize(reflect(-L, Normal));

	float distance = length(Light.Position.xyz - Position);
	float attenuation = 1.0 / (Light.Constant + Light.Linear * distance + Light.Quadratic * (distance * distance));

	vec3 ambient = vec3(texture(DiffuseTexture, TexCoord)) * Light.La;
	vec3 diffuse = vec3(texture(DiffuseTexture, TexCoord)) * Light.Ld * max(dot(L, Normal), 0);
	vec3 specular = Light.Ls * pow(max(dot(H, Normal), 0.0), Material.Shiness) * vec3(texture(SpecularTexture, TexCoord));

	vec3 LightIntensity = ambient * attenuation + diffuse * attenuation + specular * attenuation
	;

	FragColor = vec4(LightIntensity, Material.Alpha);

}