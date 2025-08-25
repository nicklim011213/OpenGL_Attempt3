#pragma once

#include "boost/filesystem.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"

#include <string>
#include <iostream>

class FileUtils
{
	FileUtils(const FileUtils&) = delete;
	FileUtils& operator=(const FileUtils&) = delete;
	FileUtils() {};

public:

	static FileUtils& GetInstance() {
		static FileUtils instance;
		return instance;
	}

	std::string ReadShader(std::string ShaderName)
	{
		boost::filesystem::path p = boost::filesystem::current_path();
		p /= "Shaders";
		p /= ShaderName;

		boost::filesystem::ifstream file(p);
		if (!file.is_open())
		{
			std::cerr << "Failed to open shader file: " << p.string() << std::endl;
		}

		std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return source;
	}

	std::string GetTexturePath()
	{
		boost::filesystem::path p = boost::filesystem::current_path();
		p /= "Textures";
		return p.string();
	}
};