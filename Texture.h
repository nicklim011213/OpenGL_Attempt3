#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include <stb_image.h>

#include "FileUtils.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class TexturePool;

class Texture
{
	int Width = 0, Height = 0, Channels = 0;
	unsigned int TextureID = 0;
	std::string Type;
	std::string Name;

public:
	Texture(std::string FileName, std::string TextureName, std::string Type)
	{
		this->Type = Type;
		this->Name = TextureName;
		std::string Path = FileUtils::GetInstance().GetTexturePath();
		std::string FullPath = Path + "/" + FileName; //TODO: Replace Windows specfic path handling
		unsigned char* data = stbi_load(FullPath.c_str(), &Width, &Height, &Channels, 0);

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
			std::cerr << "Failed to load texture: " << FullPath << std::endl;
		}

		stbi_image_free(data);
	}

public:

	unsigned int GetID() const { return TextureID; }
	const std::string& GetType() const { return Type; }
	const std::string& GetName() const { return Name; }

	void Bind(unsigned int slot = 0) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, TextureID);
	}

	void Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

class TexturePool
{
	friend class Texture;
	TexturePool() {};
	TexturePool(const TexturePool&) = delete;
	TexturePool& operator=(const TexturePool&) = delete;

	std::unordered_map<std::string, std::shared_ptr<Texture>> texturePool;

	void AddTextureToPool(const std::string& name, std::shared_ptr<Texture> texture) {
		texturePool[name] = texture;
	}

public:

	static TexturePool& GetInstance() {
		static TexturePool instance;
		return instance;
	}

	std::shared_ptr<Texture> CreateTexture(const std::string& name, const std::string& fileName, const std::string& type) {
		if (texturePool.find(name) != texturePool.end()) {
			return texturePool[name];
		}
		auto texture = std::make_shared<Texture>(fileName, name, type);
		texturePool[name] = texture;
		return texture;
	}

	std::shared_ptr<Texture> GetTexture(const std::string& name) {
		if (texturePool.find(name) != texturePool.end()) {
			return texturePool[name];
		}
		return nullptr;
	}
};