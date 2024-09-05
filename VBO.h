#pragma once

#include <glad/glad.h>
#include<glm/glm.hpp>
#include<vector>;

//Struct to standarize the vertices for use in meshes
struct Vertex
{

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;

};

class VBO
{
	public:
		GLuint ID;
		//Generates VBO and links it to the vertices
		VBO(std::vector<Vertex>& vertices);

		void Bind();
		void Unbind();
		void Delete();
};