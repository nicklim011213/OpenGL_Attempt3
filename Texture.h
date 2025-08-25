#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "FileUtils.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"


class Texture
{
	int Width = 0, Height = 0, Channels = 0;
	unsigned int TextureID = 0;

public:

	Texture(std::string FileName, std::string TextureName)
	{
		std::string Path = FileUtils::GetInstance().GetTexturePath();
		std::string FullPath = Path + "/" + FileName; //TODO: Replace Windows specfic path handling
		unsigned char* data = stbi_load(FullPath.c_str(), &Width, &Height, &Channels, 0);
		
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		int ChannelEnum = 0;
		if (Channels == 1)
		{
			ChannelEnum = GL_RED;
		}
		else if (Channels == 3)
		{
			ChannelEnum = GL_RGB;
		}
		else if (Channels == 4)
		{
			ChannelEnum = GL_RGBA;
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, ChannelEnum, Width, Height, 0, ChannelEnum, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cerr << "Failed to load texture: " << FullPath << std::endl;
		}

		stbi_image_free(data);

		TexturePool::GetInstance().AddTextureToPool(TextureName, *this);
	}

	int GetID() const {
		return TextureID;
	}
};

class TexturePool
{
	friend class Texture;
	TexturePool() {};
	TexturePool(const TexturePool&) = delete;
	TexturePool& operator=(const TexturePool&) = delete;

	std::unordered_map<std::string, Texture> texturePool;

	void AddTextureToPool(const std::string& name, Texture texture) {
		texturePool[name] = texture;
	}

public:

	static TexturePool& GetInstance() {
		static TexturePool instance;
		return instance;
	}

	int GetTextureID(const std::string& name) {
		if (texturePool.find(name) != texturePool.end()) {
			return texturePool[name].GetID();
		}
		else {
			std::cerr << "Texture not found: " << name << std::endl;
			return 0; // Return 0 if texture not found
		}
	}
};