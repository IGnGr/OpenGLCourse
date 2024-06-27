#include "Camera.h"

#define YAW_MINIMUM_ROTATION glm::radians(5.0f)

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	//Matrix initialization
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//Setting view matrix to be what the camera is looking at
	view = glm::lookAt(Position, Position + Forward, Up);

	//Calculating projection matrix to provide perspective to the view
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);
	
	//calculating the camera matrix and setting it to the uniform variable in the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));


}

void Camera::Inputs(GLFWwindow* window)
{

	//Directions
	//Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Forward;
	}
	//Backwards
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Forward;
	}
	//Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Forward, Up));
	}
	//Rigth
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Forward, Up));
	}
	//Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	//Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}


	//Sprint
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}

	//Rotation
	//Left Click
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{

		//Hiding cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


		//Avoiding sudden movements when clicking other than in the middle of the screen to rotate
		if (firstClick)
		{
			//Setting cursor to screen center
			glfwSetCursorPos(window, width / 2, height / 2);
			firstClick = false;
		}

		double mouseX;
		double mouseY;

		//Getting cursor position
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//Movement relative to window in order to rotate multiplied by sensitivity in order to control the rotation rate
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		//Pitch rotation
		glm::vec3 newForward = glm::rotate(Forward, glm::radians(-rotX), glm::normalize(glm::normalize(glm::cross(Forward, Up))));

		//Avoiding rotating indefinitely over the X axis (pitch)
		if (!((glm::angle(newForward, Up) <= YAW_MINIMUM_ROTATION) or (glm::angle(newForward, -Up) <= YAW_MINIMUM_ROTATION)))
		{
			Forward = newForward;
		}

		//Yaw rotation
		Forward = glm::rotate(Forward, glm::radians(-rotY), Up);

		//Setting the cursor to the center
		glfwSetCursorPos(window, width / 2, height / 2);

	}
	//Left Click release
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		//Cursor visible again
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;

	}
}
