#pragma once

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
	GLuint ID;
	EBO(std::vector<GLuint>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};