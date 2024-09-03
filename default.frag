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


vec4 pointLight()
{
	//Calculating intensity using a cuadratic polynomial decay
	//light direction
	vec3 lightVec = lightPos - currentPos;
	//norm of light direction vector
	float distance = length(lightVec);
	//parameters to tweak the polynomial formula
	float a = 1.0f;
	float b = 0.04f;
	//final result
	float inten = 1.0f / (a * distance * distance + b * distance + 1.0f);  
	
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

	return (texture(tex0, textureCoordinates) * (inten * difuse + ambient) + texture(tex1, textureCoordinates).r * specular * inten) * lightColor;

}

vec4 directionalLight()
{

	//ambient lighting
	float ambient = 0.2f;
	
	//diffuse lighting
	vec3 normal = normalize(normal);
	//****Note how the lightDirection vector is fixed for the directional light
	vec3 lightDirection = normalize(vec3(1.0f,1.0f,0.0f));
	float difuse = max(dot(normal, lightDirection), 0.0f);

	//specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	return (texture(tex0, textureCoordinates) * (difuse + ambient) + texture(tex1, textureCoordinates).r * specular) * lightColor;
}

vec4 spotLight()
{
	//Cones (cosine of the angle)
	//The greater, the smaller the cone
	//We use 2 to have a smooth transition from light to dark
	float outerCone = 0.5f;
	float innerCone = 0.55f;
	
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

	//Calculating the angle between a top-down light and the one from POV of the camera
	float angle = dot(vec3(0.0f,-1.0f, 0.0f), -lightDirection);
	//The intensity has to be calculated taking into account the angle calculated before and the ones from the cones
	//The greater the angle, the smaller the intensity
	float inten = clamp( (angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(tex0, textureCoordinates) * (inten * difuse + ambient) + texture(tex1, textureCoordinates).r * specular * inten) * lightColor;

}

void main()
{

	//final color
	FragColor = spotLight();


}