#include "texture.h"
// image loader

#include "stb_image.h"
#include <iostream>
#include <assert.h>
Texture::Texture(const std::string& fileName)
{
	// image w , h , num of components in it 
	int width, height, numComponents;
	// load data from stb library return sprite (image pixels with its data )
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;
	// crearte texture at the handel location and bind it 
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
// handeling image coordinates (s,t) and set suitable action : gl_repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// if resolution changed min ===> minimized ,mag===> maxmimized 
	// then interpolate the new pixel locations by linear or nearest pixel mapping 

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// get the sprite data to its location in the gpu 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// delete the image 
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind(/*unsigned int  unit*/)
{
	// to ensure that it is one of 32 textures only 
	//assert(unit >= 0 && unit <= 31);
	// determine which to activate 
	glActiveTexture(GL_TEXTURE0 );

	glBindTexture(GL_TEXTURE_2D, m_texture);
}