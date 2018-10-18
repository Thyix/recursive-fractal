#include "OpenGLHeaders.h"

ShaderLoader::ShaderLoader(void) {}
ShaderLoader::~ShaderLoader(void) {}


std::string ShaderLoader::ReadShader(char * fileName)
{
	std::string shaderCode;
	std::ifstream file(fileName, std::ios::in);

	if (!file.good()) {
		std::cout << "Le fichier est illisible: " << fileName << std::endl;
		std::terminate();
	}

	//find file length
	file.seekg(0, std::ios::end); // goto end of file
	shaderCode.resize((unsigned int)file.tellg());

	// read file
	file.seekg(0, std::ios::beg); // goto beginning of file
	file.read(&shaderCode[0], shaderCode.size());

	file.close();
	return shaderCode;
}

GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string source)
{
	int compileResult = 0;
	// create empty shader object
	GLuint shader = glCreateShader(shaderType);

	// get shader source informations
	const char* shaderCode_prt = source.c_str();
	const int shaderCode_size = source.size();

	// binding source code to new shader
	glShaderSource(shader, 1, &shaderCode_prt, &shaderCode_size);
	glCompileShader(shader);

	// validate compilation
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == GL_FALSE) {
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> shaderLog(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);
		std::cout << "Erreur de compilation des shaders" << std::endl << &shaderLog[0] << std::endl;
		return 0;
	}
	return shader;
}
GLuint ShaderLoader::CreateProgram(char * VertexShaderFileName, char * FragmentShaderFileName)
{
	std::string vertexShaderCode = ReadShader(VertexShaderFileName);
	std::string fragmentShaderCode = ReadShader(FragmentShaderFileName);

	// creating and compiling shaders
	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

	int linkResult = 0;
	// creating program handle and linking it to shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// linking program object which will be executed by the gpu
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	// validating the linking status
	if (!linkResult) {
		int infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> programLog(infoLogLength);
		glGetProgramInfoLog(program, infoLogLength, NULL, &programLog[0]);
		std::cout << "Shader loader : LINKING ERROR" << std::endl << &programLog[0] << std::endl;
		return 0;
	}
	return program;

	return GLuint();
}

