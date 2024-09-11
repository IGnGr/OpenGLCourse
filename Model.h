#pragma once

#include <nlohmann/json.hpp>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
public:
	//Loads a model from a file and assigns the attributes
	Model(const char* file);
	
	void Draw(Shader& shader, Camera& camera);


private:
	//GLTF file name
	const char* file;
	//Binary information from .bin
	std::vector<unsigned char> data;
	//Json containing the GLTF info
	json JSON; 

	//Storing meshes and transformations
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationMeshes;
	std::vector<glm::quat> rotationMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	//We store the loaded textures and names to avoid loading them twice
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	//Loads a mesh by its index
	void loadMesh(unsigned int indMesh);

	//Traverses a node recursively until all have been reached
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	//Getting binary data from file
	std::vector<unsigned char> getData();
	//Stores the binary data as floats, indices or gextures
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	//Assembling vertices from positions, normals and texture coordinates
	std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

	//Groups a group of floats into vectors.
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

};
