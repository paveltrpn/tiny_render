
#version 450 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexTxc;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(vertexPos.x, vertexPos.y, 0.0, 1.0); 
    TexCoords = vertexTxc;
}  
