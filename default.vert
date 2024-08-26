#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aText;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 textureCoordinates;
out vec3 normal;
out vec3 currentPos;

//Matrix of the camera
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	currentPos = vec3(model * vec4(aPos, 1.0f));

	//Transforming vertices into clip space via multiplying by camera matrix
   gl_Position = camMatrix * vec4(currentPos, 1.0);
   color = aColor;
   textureCoordinates = aText;
   normal = aNormal;
}