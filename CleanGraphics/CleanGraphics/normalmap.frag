#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in mat3 Tangent;

uniform sampler2D ColorTex;
uniform sampler2D NormalMapTex;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};
uniform LightInfo Light;

struct MaterialInfo {
	vec3 Ka;
	vec3 Ks;
	float Alpha;
	float Shiness;      // Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

out vec4 FragColor;

void main()
{

	// light vector
	vec3 L = normalize(Tangent * (Light.Position.xyz - Position));

	// view vector : vec3(0, 0, 0) is camera position
	vec3 V = normalize(Tangent * (vec3(0, 0, 0) - Position.xyz));

	vec4 N = texture(NormalMapTex, TexCoord);

	// reflection vector
	vec3 R = normalize(reflect(-L, N.xyz));

	vec4 Kd = texture(ColorTex, TexCoord);
	vec3 ambient = Light.La * Material.Ka;
	vec3 diffuse = Light.Ld * Kd.xyz * max(dot(L, N.xyz), 0);
	vec3 specular = Light.Ls * Material.Ks * pow(max(dot(R, V), 0), Material.Shiness);

	FragColor = vec4(ambient + diffuse + specular, Material.Alpha);
}
