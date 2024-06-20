#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
	//Initializing
	glfwInit();

	//Specifying OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Selecting CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Left lower corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Right lower corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f //Upper corner
	};




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

	//Creating vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Attaching the vertex shader source to the object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compiling the vertex shader source
	glCompileShader(vertexShader);

	//Creating fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Attaching the fragment shader source to the object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//Compiling fragment shader source
	glCompileShader(fragmentShader);

	//Creating shader program
	GLuint shaderProgram = glCreateProgram();
	//Attaching vertex and fragment shaders to shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Linking all the shaders into the shader program
	glLinkProgram(shaderProgram);

	//Deleting vertex and fragment shader objects, as they have been used already
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//References for vertex array object and vertex buffer object
	GLuint VAO, VBO;

	//Generating VAO object with 1 item
	glGenVertexArrays(1, &VAO);
	//Generating VBO object with 1 item
	glGenBuffers(1, &VBO);

	//Binding VAO (making the VAO the current Vertex Array Object)
	glBindVertexArray(VAO);

	//Binding the VBO, specifying its type GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Inserting the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Configuring the vertex attribute, so OpenGL knows how to interpret the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enabling the vertex atribute so OpenGL uses it
	glEnableVertexAttribArray(0);

	//Binding VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
		glUseProgram(shaderProgram);
		//Binding the VAO so its used by OpenGL
		glBindVertexArray(VAO);
		//Draws the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		//GLFW event handling
		glfwPollEvents();
	}

	//Deleting all created objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);


	//Deleting window
	glfwDestroyWindow(window);
	//GLFW exit to end the program
	glfwTerminate();

	return 0;
}