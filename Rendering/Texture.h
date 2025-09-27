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
	Texture(std::string FilePath, std::string TextureName, std::string Type);

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
	static bool flipped;

	std::unordered_map<std::string, std::shared_ptr<Texture>> texturePool;

	void AddTextureToPool(const std::string& name, std::shared_ptr<Texture> texture) {
		texturePool[name] = texture;
	}

public:

	static TexturePool& GetInstance() {
		static TexturePool instance;
		if (!flipped)
		{
			stbi_set_flip_vertically_on_load(true);
			flipped = true;
		}
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