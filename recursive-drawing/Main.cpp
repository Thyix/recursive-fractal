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
int levelmax = 5; // maximum level of recursion (RANDOM OR NOT???)
int nleaf = 5; // number of leafs per branch
float factor = 1.75; // controls how fast branch sizes decrease
int currentIndex = 0; // checks for the initial static branch drawing;
/*******************************/
// - KORCH SNOWFLAKE VARIABLES
GLfloat oldX = -0.875, oldY = 0.485;
std::vector<float> Scales;
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
	glm::mat4 trans = glm::mat4(1.0);
	if (currentIndex == 0) {
		sommets.push_back(vec3(0.0, -1.0, 0));
		sommets.push_back(vec3(0.0, -0.5, 0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		glUseProgram(shaderProgram);
		prepareBuffers();
		glLineWidth(1);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "trans"), 1, GL_FALSE, &trans[0][0]);
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
	// set smaller than level to increase number of branch at each level (RANDOM OR NOT???)
	for (i = 0; i < (rand() % (4 - 1 + 1) + 1); i++)
	{
		angle = rand() % 180;
		cos0 = cos(angle);
		sin0 = sin(angle);
		x2 = x + r * cos0;
		y2 = y + r * sin0;
		// if the branch is below the base / screen
		if (y2 >= -100 && y2 <= -0.3) y2 = abs(y2) - 0.5;
		sommets.push_back(vec3(x, y, 0));
		sommets.push_back(vec3(x2, y2, 0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		glUseProgram(shaderProgram);
		prepareBuffers();
		glLineWidth(1);
		trans = glm::mat4(1.0);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "trans"), 1, GL_FALSE, &trans[0][0]);
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
	GLfloat newX = oldX + length * cos(dirRad);
	GLfloat newY = oldY + length * sin(dirRad);
	glm::mat4 trans = glm::mat4(1.0);
	Scales.push_back(0.0);
	if (iter == 0) {
		sommets.push_back(vec3(oldX, oldY, 0));
		sommets.push_back(vec3(newX, newY, 0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		couleur.push_back(vec3((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
		glUseProgram(shaderProgram);
		prepareBuffers();
		glLineWidth(3);

		trans = glm::mat4(1.0);
		trans = glm::rotate(trans, Scales[currentIndex], glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(sinf(Scales[currentIndex]), sinf(Scales[currentIndex]), 1.0));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "trans"), 1, GL_FALSE, &trans[0][0]);

		glDrawArrays(GL_LINES, 0, 2);
		currentIndex += 1;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		sommets.clear();
		couleur.clear();
		oldY = newY;
		oldX = newX;
	}
	else {
		iter--;
		DrawKochSnowflake(dir, length, iter);
		dir += 60.0;
		DrawKochSnowflake(dir, length, iter);
		dir -= 120.0;
		DrawKochSnowflake(dir, length, iter);
		dir += 60.0;
		DrawKochSnowflake(dir, length, iter);
	}
	currentIndex = 0;
}

#pragma endregion

#pragma region CallBack functions
//Fonction de rappel du rendu graphique
void renduScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (currentType == FRACTAL_TREE) DrawFractalTree(0, -0.5, PI / 2, 2);
	if (currentType == KORCH_SNOWFLAKE) {
		DrawKochSnowflake(0.0, 0.02, 4);
		DrawKochSnowflake(-120.0, 0.02, 4);
		DrawKochSnowflake(120.0, 0.02, 4);
	}
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
		currentIndex = 0;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	}
}

void animationFunc(int valeur)
{
	// 16000 for clearing 3 iterations / 65000 for 4 iterations
	if (Scales.size() >= 65000) Scales.clear();
	if (currentType == KORCH_SNOWFLAKE) {
		for (int i = 0; i < Scales.size(); i++)
		{
			Scales[i] += 0.05f;
		}
		glutPostRedisplay();
	}
	glutTimerFunc(50, animationFunc, 0);
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
	glutCreateWindow("Travail Pratique 2 par William Garneau");
	/*******************/
	glewInit();
	initShaders();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	/******************/
	glutDisplayFunc(renduScene);
	glutKeyboardFunc(handlePress);
	glutTimerFunc(0, animationFunc, 0);
	glutCloseFunc(fermeture);
	/******************/
	glutMainLoop();
	/******************/
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(shaderProgram);

	return 0;
}

