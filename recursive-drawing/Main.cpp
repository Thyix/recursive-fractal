#include "OpenGLHeaders.h"

/*******************************/
// - SHADER VARIABLES
GLuint shaderProgram;
GLuint vertexbuffer;
GLuint colorbuffer;
/*******************************/
// - MEMBER VARIABLES
std::vector<glm::vec3> couleur; // store the color for each vertex
std::vector<glm::vec3> sommets; // store the vertex position
/*******************************/
// - FRACTAL TREE VARIABLES
int levelmax = 5; // maximum level of recursion
int nleaf = 5; // number of leafs per branch
float factor = 2; // controls how fast branch sizes decrease
int currentIndex = 0; // checks for the initial static branch drawing;
/*******************************/
// - KORCH SNOWFLAKE VARIABLES
GLfloat oldX = -0.7, oldY = -0.5;
/*******************************/

FractalTypes currentType;

typedef unsigned char uchar;

#pragma region Shader Management

void prepareBuffers()
{
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sommets.size() * sizeof(float) * 3, sommets.data(), GL_STREAM_DRAW);


	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL           // array buffer offset
	);

	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, couleur.size() * sizeof(float) * 3, couleur.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL
	);

	glEnableVertexAttribArray(1);
}

//Fonction d'initialisation des shaders
void initShaders()
{
	ShaderLoader shaderLoader;
	shaderProgram = shaderLoader.CreateProgram((char*)"vertexShader.glsl", (char*)"fragmentShader.glsl");
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &colorbuffer);
}
#pragma endregion

#pragma region Fractal Drawing

void DrawFractalTree(float x, float y, float angle0, float level) {
	if (currentIndex == 0) {
		sommets.push_back(vec3(0.0, -1.0, 0));
		sommets.push_back(vec3(0.0, -0.5, 0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		glUseProgram(shaderProgram);
		prepareBuffers();
		glLineWidth(3);
		glDrawArrays(GL_LINES, 0, 2);
		currentIndex += 1;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		sommets.clear();
		couleur.clear();
	}
	if (level > levelmax) return;
	float r = 0.75 / pow(factor, level - 1);
	float angle;
	float x2, y2, cos0, sin0;
	int i;
	for (i = 0; i < level; i++)
	{

		angle = rand() % 180;
		cos0 = cos(angle);
		sin0 = sin(angle);
		x2 = x + r * cos0;
		y2 = y + r * sin0;
		// if the branch is below the tree / screen
		if (y2 >= -100 && y2 <= -0.3) y2 = abs(y2) - 0.5;
		sommets.push_back(vec3(x, y, 0));
		sommets.push_back(vec3(x2, y2, 0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		glUseProgram(shaderProgram);
		prepareBuffers();
		glLineWidth(3);
		glDrawArrays(GL_LINES, 0, 2);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		sommets.clear();
		couleur.clear();

		DrawFractalTree(x2, y2, atan2((y2 - y), (x2 - x)), level + 1);
	}
	currentIndex = 0;
}

void DrawKochSnowflake(GLfloat dir, GLfloat length, GLint iter) {
	GLdouble dirRad = 0.0174533 * dir;
	if (iter == 0) {
		sommets.push_back(vec3(0.0, -1.0, 0));
		sommets.push_back(vec3(0.0, -0.5, 0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		glUseProgram(shaderProgram);
		prepareBuffers();
		glLineWidth(3);
		glDrawArrays(GL_LINES, 0, 2);
		currentIndex += 1;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		sommets.clear();
		couleur.clear();
	}
}

#pragma endregion

#pragma region CallBack functions
//Fonction de rappel du rendu graphique
void renduScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (currentType == FRACTAL_TREE) DrawFractalTree(0, -0.5, PI / 2, 2);
	glFlush();
}

void handlePress(uchar key, int x, int y) {
	switch (key) {
	case 'k':
		currentType = KORCH_SNOWFLAKE;
		glutPostRedisplay();
		break;
	case 'a':
		currentType = FRACTAL_TREE;
		levelmax = (rand() % 4) + 2;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	}
}

/******************************************/
void fermeture()
{
	glutLeaveMainLoop();
}
#pragma endregion


//Fonction principale d'initialisation et d'exécution
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Travail Pratique 2");

	/*******************/

	glewInit();
	initShaders();
	glClearColor(0.0, 0.0, 0.0, 1.0);

	/******************/

	glutDisplayFunc(renduScene);
	glutKeyboardFunc(handlePress);
	glutCloseFunc(fermeture);

	glutMainLoop();

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(shaderProgram);

	return 0;
}

