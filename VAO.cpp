#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO VBO, GLuint layout)
{
	Bind();

	//Configuring the vertex attribute, so OpenGL knows how to interpret the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enabling the vertex atribute so OpenGL uses it
	glEnableVertexAttribArray(0);


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
