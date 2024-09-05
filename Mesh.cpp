#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;


	//Binding VAO so the instances of VBO and EBO point to it
	Mesh::VAO.Bind();

	//Instancing VBO and EBO
	VBO VBOObject = VBO(vertices);
	EBO EBOObject = EBO(indices);

	//Linking VAO to VBO and specifying layout
	//Position
	Mesh::VAO.LinkAttrib(VBOObject, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	//Color
	Mesh::VAO.LinkAttrib(VBOObject, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	//Texture
	Mesh::VAO.LinkAttrib(VBOObject, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	//Normals 
	Mesh::VAO.LinkAttrib(VBOObject, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	//Unbinding once everything is instanciated
	Mesh::VAO.Unbind();
	VBOObject.Unbind();
	EBOObject.Unbind();

}

void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.Activate();
	Mesh::VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;

		if (type == "difuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].TexUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();

		glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shader, "camMatrix");

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	}


}
