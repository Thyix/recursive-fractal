#version 430 core

in vec3 fragCouleur;
out vec4 color;

void main(void)
{
	color = vec4(fragCouleur, 1.0);
}
