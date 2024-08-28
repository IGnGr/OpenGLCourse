#version 330 core
out vec4 FragColor;

//importing color from vertex shader
in vec3 color;
//importing texture coordinates from vertex shader
in vec2 textureCoordinates;
//importing normal values from vertex shader
in vec3 normal;
//importing currentPosition value from vertex shader
in vec3 currentPos;

//Getting texture unit from main function
uniform sampler2D tex0;

uniform sampler2D tex1;

//geting lightColor information from main function
uniform vec4 lightColor;
//getting lightPosition information from main function
uniform vec3 lightPos;
//gettign cameraPosition value from main function
uniform vec3 camPos;

void main()
{
	//ambient lighting
	float ambient = 0.2f;
	
	//diffuse lighting
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);
	float difuse = max(dot(normal, lightDirection), 0.0f);

	//specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	//final color
	FragColor = (texture(tex0, textureCoordinates) * (difuse + ambient) + texture(tex1, textureCoordinates).r * specular) * lightColor;


}