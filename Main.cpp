#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"


GLfloat vertices[] =
{  // Coordinates					//Color					//Texture mapping coordinates
	-0.5f,  0.0f , 0.5f,            0.83f, 0.0f, 0.0f,		0.0f, 0.0f, 
	-0.5f,  0.0f ,-0.5f,            0.0f, 1.0f, 0.0f,		5.0f, 0.0f, 
	 0.5f,  0.0f ,-0.5f,            0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	
	 0.5f,  0.0f , 0.5f,            1.0f, 1.0f, 1.0f,		5.0f, 0.0f,  
	 0.0f,  0.8f , 0.0f,            1.0f, 1.0f, 1.0f,		2.5f, 5.0f  

};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

const unsigned int width = 800;
const unsigned int height = 800;

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
	VAOObject.LinkAttrib(VBOObject, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	//Color
	VAOObject.LinkAttrib(VBOObject, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*) (3*sizeof(float)));
	//Texture
	VAOObject.LinkAttrib(VBOObject, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)  (6 * sizeof(float)));

	//Unbinding once everything is instanciated
	VAOObject.Unbind();
	VBOObject.Unbind();
	EBOObject.Unbind();

	//Getting the uniform value for scale from shaders
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Textures

	Texture catTexture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	catTexture.TexUnit(shaderProgram, "tex0", 0);


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

	//Main loop to be able to see the window
	while (!glfwWindowShouldClose(window))
	{
		//Specifying the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		//Cleaning back buffer and depth buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Specifying shader program to use
		shaderProgram.Activate();

		double currentTime = glfwGetTime();

		if (currentTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = currentTime;
		}

		//Initializing matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		//Calculating the model, view and projection matrices
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);


		//Inserting the matrices into the vertex shader. via uniform 
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		//Assigning value to the uniform float unIID. Must be done after the shader program is activated.
		glUniform1f(uniID, 1.5f);
		catTexture.Bind();

		//Binding the VAO so its used by OpenGL
		VAOObject.Bind();
		//Draws the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		//GLFW event handling
		glfwPollEvents();
	}

	//Deleting all created objects
	VBOObject.Delete();
	VAOObject.Delete();
	EBOObject.Delete();
	shaderProgram.Delete();
	catTexture.Delete();
	//Deleting window
	glfwDestroyWindow(window);
	//GLFW exit to end the program
	glfwTerminate();

	return 0;
}