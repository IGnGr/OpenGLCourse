#include "Model.h"

Model::Model(const char* file)
{
	//Creating JSON from file
	std::string text = getFileContents(file);
	JSON = json::parse(text);

	//Getting binary data
	Model::file = file;
	data = getData();

	//Traversing all nodes
	traverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	//Drawing all meshes
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
	}
}

void Model::loadMesh(unsigned int indMesh)
{
	//Getting all accessor indices
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	//Getting position, normal and texture UVs from the accessor indices 
	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

	//Assembling vertex, index and texture vectors
	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	std::vector<Texture> textures = getTextures();

	//Creating mesh and storing it
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	//Obtaining current node
	json node = JSON["nodes"][nextNode];

	//Default translation value
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

	//If theres a translation defined, we store it and update our value
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
		{
			transValues[i] = node["translation"][i];
		}
		translation = glm::make_vec3(transValues);
	}

	//Default rotation value
	glm::quat rotation = glm::angleAxis(glm::radians(-45.f), glm::vec3(.0f, 1.0f, 0.0f));

	//If theres a rotation defined, we store it and update our value
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3], node["rotation"][0],node["rotation"][1], node["rotation"][2]
		};

		rotation = glm::make_quat(rotValues);
	}

	//Default scale value
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	//If theres a scale defined, we store it and update our value
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];

		for (int i = 0; i < node["scale"].size(); i++)
		{
			scaleValues[i] = node["scale"][i];
		}
		scale = glm::make_vec3(scaleValues);
	}

	//Default matrix value
	glm::mat4 matNode = glm::mat4(1.0f);

	//If theres a matrix defined, we store it and update our value
	//Basically for parent-children transformations
	if (node.find("matrix") != node.end())
	{
		float matValues[16];

		for (int i = 0; i < node["matrix"].size(); i++)
		{
			matValues[i] = node["matrix"][i];
		}

		matNode = glm::make_mat4(matValues);
	}

	//Calculating translation, rotation and scale matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	//Obtaining final matrix with previous values
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	//In case theres a mesh, we load it
	if (node.find("mesh") != node.end())
	{
		translationMeshes.push_back(translation);
		rotationMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matrix);

		loadMesh(node["mesh"]);
	}

	//In case there are any children, we process them recursively 
	if (node.find("children") != node.end())
	{
		for (int i = 0; i < node["children"].size(); i++)
		{
			traverseNode(node["children"][i], matNextNode);
		}
	}
}

std::vector<unsigned char> Model::getData()
{
	//Variables to store the binary information as text and the uri
	std::string bytesText;
	std::string uri = JSON["buffers"][0]["uri"];

	//Storing the file information
	std::string fileString = std::string(file);
	std::string fileDirectory = fileString.substr(0, fileString.find_last_of('/') + 1);
	bytesText = getFileContents((fileDirectory + uri).c_str());

	//Inserting the data to a vector
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());

	return data;
}

std::vector<float> Model::getFloats(json accessor)
{
	std::vector<float> floatVec;

	//Getting properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	//Getting properties from bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	//Determining the type of value to treat it accordingly
	unsigned int numperVert; 
	if (type == "SCALAR") numperVert = 1;
	else if (type == "VEC2") numperVert = 2;
	else if (type == "VEC3") numperVert = 3;
	else if (type == "VEC4") numperVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3 or VEC4)");

	//Traversing the data with the previously defined properties and storing it in memory
	unsigned int dataBegining = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numperVert;
	for (unsigned int i = dataBegining; i < dataBegining + lengthOfData; i)
	{
		//Raw data as 4 char (4 bytes)
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		//"Casting" to float
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;

}

std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	//Getting properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	//Getting properties from bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView.value("byteOffset",0);

	//Traversing the data with the previously defined properties and storing it in memory
	unsigned int dataBegining = byteOffset + accByteOffset;

	//Determining type of data by identifier
	//5125 = unsigned int
	if(componentType == 5125)
	{
		for (unsigned int i = dataBegining; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(float));
			indices.push_back((GLuint) value);
		}
	} 
	//5123 = unsigned short
	else if (componentType == 5123)
	{
		for (unsigned int i = dataBegining; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++]};
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	//5122 = short
	else if (componentType == 5122)
	{
		for (unsigned int i = dataBegining; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}
	
	return indices;

}

std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;

	std::string fileString = std::string(file);
	std::string fileDirectory = fileString.substr(0, fileString.find_last_of('/') + 1);

	//Traversing all images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		//uri of current texture
		std::string texPath = JSON["images"][i]["uri"];

		//Checking if the texture has been loaded already
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			//Diffuse texture
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			//Specular texture
			else if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs)
{
	std::vector<Vertex> vertices;

	for (int i = 0; i < positions.size(); i++)
	{
		vertices.push_back(Vertex{ positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), texUVs[i] });
	}

	return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;

	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}

	return vectors;

}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;

	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	return vectors;

}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;

	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	return vectors;

}
