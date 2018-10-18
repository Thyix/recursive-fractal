class ShaderLoader
{
private:
	std::string ReadShader(char* fileName);
	GLuint CreateShader(GLenum shaderType, std::string source);
public:
	ShaderLoader();
	~ShaderLoader();
	GLuint CreateProgram(char* VertexShaderFileName, char* FragmentShaderFileName);
};


