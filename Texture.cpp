#include "Texture.h"


Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColCh;

	//Flips the image so it doesn't show upside down
	stbi_set_flip_vertically_on_load(true);

	//Reads an image
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	//Generating Texture object
	glGenTextures(1, &ID);
	//Assigning the texture to a Texture unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	//Configuring the type of algorithm to be used when changing the image size
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Configuring how OpenGL handles repetition of the image
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Assigning the image to OpenGL texture object
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	glGenerateMipmap(texType);

	//Deleting the image data as it's already copied to OpenGL texture object
	stbi_image_free(bytes);

	//Unbinding the OpenGL texture object so it's not modified anymore
	glBindTexture(texType, 0);
}

void Texture::TexUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint tex0Uniform = glGetUniformLocation(shader.ID, uniform);
	//Activating shader before changing the unfiform value
	shader.Activate();
	glUniform1i(tex0Uniform, unit);


}


void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}