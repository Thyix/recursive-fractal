#version 430 core

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 vboColor;
out vec4 Color;

void main(void)
{
	gl_Position = Position;
	Color = vboColor;
}