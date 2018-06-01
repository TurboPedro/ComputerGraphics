#version 400

in vec4 Position;
in vec3 Normal;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 La;
	vec3 Ld;
	vec3 Ls;

	float Constant;
	float Linear;
	float Quadratic;
};

struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Alpha;
	float Shiness;      // Specular shininess factor (phong exponent)
};

uniform vec4 LightPosition;
uniform LightInfo Light;

uniform MaterialInfo Material;

out vec4 FragColor;

void main()
{
	// light vector
	vec3 L = normalize(Light.Position - Position).xyz;

	// view vector : vec3(0, 0, 0) is camera position
	vec3 V = normalize(vec3(0, 0, 0) - Position.xyz);

	// reflection vector
	vec3 H = normalize(V + L);
	vec3 R = normalize(reflect(-L, Normal));

	vec3 ambient = Material.Ka * Light.La;
	vec3 diffuse = Material.Kd * Light.Ld * max(dot(L, Normal), 0);
	vec3 specular = Material.Ks * Light.Ls * pow(max(dot(H, Normal), 0.0), Material.Shiness);

	float distance = length(Light.Position.xyz - Position.xyz
	);
	float attenuation = 1.0 / (Light.Constant + Light.Linear * distance + Light.Quadratic * (distance * distance));

	vec3 LightIntensity = (ambient + diffuse + specular) * attenuation;

	FragColor = vec4(LightIntensity, Material.Alpha);
}