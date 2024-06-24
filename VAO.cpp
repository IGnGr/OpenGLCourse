#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	Bind();

	//Configuring the vertex attribute, so OpenGL knows how to interpret the VBO
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	//Enabling the vertex atribute so OpenGL uses it
	glEnableVertexAttribArray(layout);


	Unbind();
}

void VAO::Bind()
{
	//Binding VAO (making the VAO the current Vertex Array Object)
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
