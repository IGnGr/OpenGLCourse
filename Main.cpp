#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>


#include "ShaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"


GLfloat vertices[] =
{  // Coordinates					//Color					//Texture mapping coordinates
	-0.5f, -0.5f , 0.0f,            1.0f, 0.0f, 0.0f,		0.0f, 0.0f, //Left lower corner
	-0.5f,  0.5f , 0.0f,            0.0f, 1.0f, 0.0f,		0.0f, 1.0f, //Left upper corner
	 0.5f,  0.5f , 0.0f,            0.0f, 0.0f, 1.0f,		1.0f, 1.0f,	//Right upper corner
	 0.5f, -0.5f , 0.0f,            1.0f, 1.0f, 1.0f,		1.0f, 0.0f  //Right lower corner

};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
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
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Course", NULL, NULL);


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
	glViewport(0, 0, 800, 800);

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

	Texture catTexture("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	catTexture.TexUnit(shaderProgram, "tex0", 0);


	//Setting the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	//Cleaning the back buffer and assigning the new color
	glClear(GL_COLOR_BUFFER_BIT);

	//Swapping buffers so we see the one with the custom color
	glfwSwapBuffers(window);

	//Main loop to be able to see the window
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Specifying shader program to use
		shaderProgram.Activate();
		//Assigning value to the uniform float unIID. Must be done after the shader program is activated.
		glUniform1f(uniID, 1.5f);
		catTexture.Bind();

		//Binding the VAO so its used by OpenGL
		VAOObject.Bind();
		//Draws the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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