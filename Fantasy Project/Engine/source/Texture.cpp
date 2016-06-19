#include <FreeImage.h>
#include <Texture.h>

static bool Glew_Initialized = false;

////
Texture LoadTexture(char *imagePath)
{
	if (!Glew_Initialized)
	{
		bool res = ReloadGlew();
		Glew_Initialized = true;
	}

	FreeImage_Initialise();

	Texture texture;

	FREE_IMAGE_FORMAT imageFormat = FIF_UNKNOWN;

	FIBITMAP *dib(0);

	unsigned char *bits(0);

	imageFormat = FreeImage_GetFileType(imagePath);

	if (imageFormat == FIF_UNKNOWN)
	{
		imageFormat = FreeImage_GetFIFFromFilename(imagePath);
	}

	if (imageFormat == FIF_UNKNOWN)
	{
		return Texture{};
	}

	if (FreeImage_FIFSupportsReading(imageFormat))
	{
		dib = FreeImage_Load(imageFormat, imagePath);
	}

	bits = FreeImage_GetBits(dib);

	texture.Width = FreeImage_GetWidth(dib);
	texture.Height = FreeImage_GetHeight(dib);
	
	glGenTextures(1, &texture.TextureHandle);

	glBindTexture(GL_TEXTURE_2D, texture.TextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width, texture.Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(dib);

	FreeImage_DeInitialise();

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
