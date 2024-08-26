#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 textureCoordinates;
in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	float ambient = 0.2f;
	
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);
	float difuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;


	FragColor = texture(tex0, textureCoordinates) * lightColor * (difuse + ambient + specular);


}