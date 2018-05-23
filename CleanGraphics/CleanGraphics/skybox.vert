#version 400

layout(location = 0) in vec3 VertexPosition;

out vec3 ReflectDir;

uniform bool DrawSkyBox;
uniform mat4 mvp;

void main()
{
    ReflectDir = VertexPosition;
    gl_Position = mvp * vec4(VertexPosition,1.0);
  
}