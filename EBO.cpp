#include "EBO.h"

EBO::EBO(std::vector<GLuint>& vertices)
{
	//Generating buffer
	glGenBuffers(1, &ID);
	//Binding the EBO, specifying its type GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	//Inserting the vertices into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);

}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
