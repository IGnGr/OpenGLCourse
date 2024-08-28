#define GLM_ENABLE_EXPERIMENTAL

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"ShaderClass.h"
#include"VBO.h"
#include"VAO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"


const unsigned int width = 800;
const unsigned int height = 800;


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};


GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
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

	//Instancing shader program
	Shader shaderProgram = Shader("default.vert", "default.frag");

	//References for vertex array object, vertex buffer object and element buffer object
	VAO VAOObject;
	//Binding VAO so the instances of VBO and EBO point to it
	VAOObject.Bind();

	//Instancing VBO and EBO
	VBO VBOObject = VBO(vertices, sizeof(vertices));
	EBO EBOObject = EBO(indices, sizeof(indices));

	//Linking VAO to VBO and specifying layout
	//Position
	VAOObject.LinkAttrib(VBOObject, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	//Color
	VAOObject.LinkAttrib(VBOObject, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*) (3*sizeof(float)));
	//Texture
	VAOObject.LinkAttrib(VBOObject, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)  (6 * sizeof(float)));
	//Normals 
	VAOObject.LinkAttrib(VBOObject, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	//Unbinding once everything is instanciated
	VAOObject.Unbind();
	VBOObject.Unbind();
	EBOObject.Unbind();

	//Getting the uniform value for scale from shaders
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//LIGHTING

	//Instancing shader program
	Shader lightShader = Shader("light.vert", "light.frag");

	//References for vertex array object, vertex buffer object and element buffer object
	VAO lightVAO;
	//Binding VAO so the instances of VBO and EBO point to it
	lightVAO.Bind();

	//Instancing VBO and EBO
	VBO lightVBO = VBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO = EBO(lightIndices, sizeof(lightIndices));

	//Linking VAO to VBO and specifying layout
	//Position
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

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

	//Textures
	Texture brickTexture("resources/textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTexture.TexUnit(shaderProgram, "tex0", 0);

	Texture planksSpec("resources/textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	planksSpec.TexUnit(shaderProgram, "tex1", 1);

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

		//Exports the camera position to the fragment shader to be able to calculate specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		//Exports the camera matrix to the vertex shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");


		//Binding the texture so it renders
		brickTexture.Bind();
		planksSpec.Bind();

		//Binding the VAO so its used by OpenGL
		VAOObject.Bind();
		//Draws the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);




		glfwSwapBuffers(window);

		//GLFW event handling
		glfwPollEvents();
	}

	//Deleting all created objects
	VBOObject.Delete();
	VAOObject.Delete();
	EBOObject.Delete();
	shaderProgram.Delete();
	brickTexture.Delete();
	planksSpec.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	//Deleting window
	glfwDestroyWindow(window);
	//GLFW exit to end the program
	glfwTerminate();

	return 0;
}