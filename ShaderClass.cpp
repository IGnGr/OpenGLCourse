#include "ShaderClass.h"

std::string getFileContents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string content;
		in.seekg(0, std::ios::end);
		content.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&content[0], content.size());
		in.close();
		return content;

	}
	throw (errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = getFileContents(vertexFile);
	std::string framentCode = getFileContents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = framentCode.c_str();

	//Creating vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Attaching the vertex shader source to the object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//Compiling the vertex shader source
	glCompileShader(vertexShader);

	//Creating fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Attaching the fragment shader source to the object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//Compiling fragment shader source
	glCompileShader(fragmentShader);

	//Creating shader program
	ID = glCreateProgram();
	//Attaching vertex and fragment shaders to shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//Linking all the shaders into the shader program
	glLinkProgram(ID);

	//Deleting vertex and fragment shader objects, as they have been used already
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}