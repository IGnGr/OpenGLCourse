#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"

class Camera
{
	public:
		glm::vec3 Position;
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);
	
		//Variable to avoid issues when clicking other than in the middle of the screen
		bool firstClick = true;

		//Size of the camera port
		int width;
		int height;

		//Camera movement speed
		float speed = 0.1f;
		//Camera rotation speed
		float sensitivity = 100.0f;

		Camera(int width, int height, glm::vec3 position);

		//Calculates camera matrix
		void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
		void Matrix(Shader& shader, const char* uniform);

		//Handles player inputs
		void Inputs(GLFWwindow* window);
		
};