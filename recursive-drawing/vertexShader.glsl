#version 430 core
layout(location = 0) in vec3 in_sommet;
layout(location = 1) in vec3 in_couleur;

out vec3 fragCouleur;
uniform mat4 trans;

void main()
{
	if (trans != 0) {
		gl_Position = trans * vec4(in_sommet, 1.0);
	}
	else {
		gl_Position = vec4(in_sommet, 1.0);
	}
	fragCouleur = in_couleur;
}
