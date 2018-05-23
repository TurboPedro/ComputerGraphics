#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D Tex1;
uniform sampler2D Tex2;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};
uniform LightInfo Light;

struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
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

	vec3 ambient = Material.Ka * Light.La;
	vec3 diffuse = Material.Kd * Light.Ld * max(dot(L, Normal), 0);
	vec3 specular = Material.Ks * Light.Ls * pow(max(dot(H, Normal), 0.0), Material.Shiness);
	vec4 texColor1 = texture(Tex1, TexCoord);
	vec4 texColor2 = texture(Tex2, TexCoord);
	vec4 texColor = mix(texColor2, texColor1, texColor2.a);

	vec3 LightIntensity = ((vec4( ambient + diffuse, 1.0 ) * texColor) + vec4(specular, 1.0)).xyz;

	FragColor = vec4(LightIntensity, Material.Alpha);

}