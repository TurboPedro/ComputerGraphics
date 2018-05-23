#version 400

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 mvp;

out vec4 Position;
out vec3 Normal;

void main(void)
{
   //a point in camera coord
   Position = ModelViewMatrix * vec4(VertexPosition, 1.0);

   //transforming normal
   Normal = normalize(NormalMatrix * VertexNormal);

   gl_Position = mvp * vec4(VertexPosition, 1.0);
}