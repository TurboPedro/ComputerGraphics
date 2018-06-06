#version 400

layout(location = 0) in vec3 VertexPosition;

uniform mat4 mvp;

void main(void)
{
   gl_Position = mvp * vec4(VertexPosition, 1.0);
}