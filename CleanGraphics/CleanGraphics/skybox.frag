#version 400

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Alpha;
	float Shiness;      // Specular shininess factor (phong exponent)
};

in vec3 ReflectDir;

uniform samplerCube CubeMapTex;

uniform bool DrawSkyBox;
out vec4 FragColor;

void main() {
    // Access the cube map texture
    vec4 cubeMapColor = texture(CubeMapTex, ReflectDir);

    FragColor = cubeMapColor;
}