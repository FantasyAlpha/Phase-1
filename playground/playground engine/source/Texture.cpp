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

	std::vector<unsigned char> image;
	DataFile file;
	LoadFile(imagePath, &file);
	unsigned char *data = (unsigned char*)file.Data;
	decodePNG(image, texture.Width, texture.Height, (unsigned char*)file.Data, file.Length);

	std::vector<unsigned char> newImage;
	
	for (int h = texture.Height - 1; h >= 0; h--)
	{
		for (int w = 0; w < texture.Width * 4; w ++)
		{
			newImage.push_back(image[w + 0 + (h * texture.Width * 4)]);
		}
	}

	glGenTextures(1, &texture.TextureHandle);

	glBindTexture(GL_TEXTURE_2D, texture.TextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width, texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(newImage[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
