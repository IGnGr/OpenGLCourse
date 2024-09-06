#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aText;

out vec3 currentPos;
out vec3 color;
out vec2 textureCoordinates;
out vec3 normal;


//Matrix of the camera
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;


void main()
{
	currentPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
	normal = aNormal;
	color = aColor;
	textureCoordinates = mat2(0.0, -1.0, 1.0, 0.0) * aText;

   	//Transforming vertices into clip space via multiplying by camera matrix
   gl_Position = camMatrix * vec4(currentPos, 1.0);
}