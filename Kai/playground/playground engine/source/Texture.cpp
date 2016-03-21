#include <Texture.h>

static bool Glew_Initialized = false;

////
Texture LoadTexture(char *imagePath)
{
	if (!Glew_Initialized){
		bool res = ReloadGlew();
		Glew_Initialized = true;
	}
	Texture texture;
	unsigned long TextureWidth, TextureHeight;

	std::vector<unsigned char> image;
	DataFile file;
	LoadFile(imagePath, &file);
	unsigned char *data = (unsigned char*)file.Data;
	decodePNG(image, TextureWidth, TextureHeight, (unsigned char*)file.Data, file.Length);
	glGenTextures(1, &texture.TextureHandle);

	glBindTexture(GL_TEXTURE_2D, texture.TextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);
	GLenum error = glGetError();

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

void BindTexture(Texture *texture)
{
	if (texture->TextureHandle)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->TextureHandle);
	}
}

void UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
