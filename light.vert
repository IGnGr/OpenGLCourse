#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

//Matrix of the camera
uniform mat4 camMatrix;



void main()
{
	//Transforming vertices into clip space via multiplying by camera matrix
   gl_Position = camMatrix * model * vec4(aPos, 1.0);
}