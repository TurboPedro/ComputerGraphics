#version 430

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTextureCoor;
layout(location = 3) in vec3 VertexTangent;
layout(location = 4) in vec3 VertexBitTangent;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out mat3 Tangent;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 mvp;

void main()
{
	vec3 norm = normalize(NormalMatrix * VertexNormal);
	vec3 tang = normalize(NormalMatrix * VertexTangent);
	vec3 bitTang = normalize(NormalMatrix * VertexBitTangent);
	
	Tangent = mat3(tang.x, norm.x, bitTang.x,
					tang.y, norm.y, bitTang.y,
					tang.z, norm.z, bitTang.z);
	TexCoord = VertexTextureCoor;
	Normal = normalize(NormalMatrix * VertexNormal);
	Position = vec3(ModelViewMatrix * vec4(VertexPosition,1.0));

	gl_Position = mvp * vec4(VertexPosition,1.0);
}
