#include "Texture.h"

bool TexturePool::flipped = false;

Texture::Texture(std::string FilePath, std::string TextureName, std::string Type)
{
	this->Type = Type;
	this->Name = TextureName;
	unsigned char* data = stbi_load(FilePath.c_str(), &Width, &Height, &Channels, 0);

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	int ChannelEnum = (Channels == 1) ? GL_RED :
		(Channels == 3) ? GL_RGB :
		(Channels == 4) ? GL_RGBA : 0;

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, ChannelEnum, Width, Height, 0, ChannelEnum, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture: " << FilePath << std::endl;
	}

	stbi_image_free(data);
}