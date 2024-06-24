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
	CompileErrors(vertexShader, "VERTEX");

	//Creating fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Attaching the fragment shader source to the object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//Compiling fragment shader source
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");

	//Creating shader program
	ID = glCreateProgram();
	//Attaching vertex and fragment shaders to shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//Linking all the shaders into the shader program
	glLinkProgram(ID);
	CompileErrors(ID, "PROGRAM");


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

void Shader::CompileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}