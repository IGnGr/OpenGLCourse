#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aText;

out vec3 color;
out vec2 textureCoordinates;

//Matrix of the camera
uniform mat4 camMatrix;

void main()
{
	//Transforming vertices into clip space via multiplying by camera matrix
   gl_Position = camMatrix * vec4(aPos, 1.0);
   color = aColor;
   textureCoordinates = aText;
}