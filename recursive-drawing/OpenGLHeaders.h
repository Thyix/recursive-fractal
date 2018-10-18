// - DOCUMENTATION
// https://xp-dev.com/sc/93479/2/%2Fsnowflake.c
// https://github.com/dchhetri/Recursive-Play-with-OpenGL
// https://github.com/brunofalmeida/RecursiveGraphics-GLUT


// --- STRUCTURES
struct Color {
	float red;
	float green;
	float blue;
	Color() {
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	Color(float pRed, float pGreen, float pBlue) {
		red = pRed;
		green = pGreen;
		blue = pBlue;
	}
};

struct Position {
	float x;
	float y;
	Position(float pX, float pY) {
		x = pX;
		y = pY;
	}
};

// ENUMERATIONS
enum FractalTypes {
	NULL,
	FRACTAL_TREE,
	KORCH_SNOWFLAKE
};

// -- MODULES
#include <fstream>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include "Dependencies\glm\glm\glm.hpp"
#include "Dependencies\glm\glm\gtx\transform.hpp"
#include <vector>
#include <cstdlib>
#include <math.h> 
#include <ctime>

#include "ShaderLoader.h"
#include "GlutWindow.h"

// - NAMESPACES
using namespace std;
using namespace glm;

// - CONSTANTS
const float PI = 3.14159265359;