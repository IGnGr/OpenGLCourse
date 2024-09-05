#define GLM_ENABLE_EXPERIMENTAL

#include "Mesh.h"

const unsigned int width = 800;
const unsigned int height = 800;


// Vertices coordinates
Vertex vertices[] = {
	//COORDINATES						/ NORMALS					/Color							/Textures UV       //
	  Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
	  Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
	  Vertex{glm::vec3(1.0f, 0.0f, -1.0f) , glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)},
	  Vertex{glm::vec3(1.0f, 0.0f,  1.0f) , glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] = {
	0, 1, 2,
	0, 2, 3
};


Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex {glm::vec3(-0.1f,-0.1f,  0.1f)},
	Vertex {glm::vec3(-0.1f,-0.1f, -0.1f)},
	Vertex {glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex {glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex {glm::vec3(-0.1f, 0.1f,  0.1f)},
	Vertex {glm::vec3(-0.1f, 0.1f, -0.1f)},
	Vertex {glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex {glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};



int main()
{
	//Initializing
	glfwInit();

	//Specifying OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Selecting CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//Creating window 800 x 800 pixels, with "OpenGL course" title
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Course", NULL, NULL);


	//Error handling in the window creation
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Introducing the window in the current context
	glfwMakeContextCurrent(window);

	//Loading GLAD to configure OpenGL
	gladLoadGL();

	//Specifying the viewport, from 0,0 to 800,800
	glViewport(0, 0, width, height);

	Texture textures[]
	{
		Texture("resources/textures/planks.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("resources/textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	//Instancing shader program
	Shader shaderProgram = Shader("default.vert", "default.frag");

	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	Mesh floor = Mesh(verts, ind, tex);

	//LIGHTING

	//Instancing shader program
	Shader lightShader = Shader("light.vert", "light.frag");

	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

	Mesh light = Mesh(lightVerts, lightInd, tex);

	//Defining light source color
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f,1.0f, 1.0f);

	//Defining light position
	glm::vec3 lightPos = glm::vec3(0.5, 0.5f, 0.5f);

	//initializing light model matrix
	glm::mat4 lightModel = glm::mat4(1.0f);

	//calculating translation matrix via light model matrix and position
	lightModel = glm::translate(lightModel, lightPos);

	//Defining pyramid position
	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	//Initializing pyramid model matrix
	glm::mat4 pyramidModel = glm::mat4(1.0f);

	//calculating translation matrix via pyramid model matrix and position
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	//activation of the shader required in order to set uniforms
	lightShader.Activate();

	//Exporting model matrix to lighting vertex shader
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	//Exporting lighing color to lighting fragment shader
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	//activation of the shader required in order to set uniforms
	shaderProgram.Activate();

	//Exporting model matrix to pyramid vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	//Exporting light color to pyramid fragment shader
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	//Exporting light source position to pyramid fragment shader
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	//LIGHTING END


	//Setting the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	//Cleaning the back buffer and assigning the new color
	glClear(GL_COLOR_BUFFER_BIT);

	//Swapping buffers so we see the one with the custom color
	glfwSwapBuffers(window);

	//Pyramid rotation variables
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	//Enabling depth buffer
	glEnable(GL_DEPTH_TEST);


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	//Main loop to be able to see the window
	while (!glfwWindowShouldClose(window))
	{
		//Specifying the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		//Cleaning back buffer and depth buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Specifying shader program to use
		shaderProgram.Activate();

		//Handling player inputs
		camera.Inputs(window);

		//Updates camera matrix and exports it into the vertex shader
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);


		glfwSwapBuffers(window);

		//GLFW event handling
		glfwPollEvents();
	}

	//Deleting all created objects
	shaderProgram.Delete();
	lightShader.Delete();

	//Deleting window
	glfwDestroyWindow(window);
	//GLFW exit to end the program
	glfwTerminate();

	return 0;
}