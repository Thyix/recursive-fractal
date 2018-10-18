#include "OpenGLHeaders.h"

GlutWindow::GlutWindow() {}

GlutWindow::GlutWindow(int windowWidth, int windowHeight, char* windowName)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow((char*)windowName);
	glewInit();

	cout << "Bienvenue dans l'outil de dessin" << endl << "Pour commencer, veuillez choisir un outil et une couleur." << endl << endl;
}