#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <boost/filesystem.hpp>

#include "FileUtils.h"

class Shader;
class ShaderProgram;
enum ShaderType
{
	VERTEX,
	FRAGMENT
};

typedef boost::filesystem::path path;

class Shader
{
	friend class ShaderFactory;

protected:

	std::string Name;
	std::string SourceCode;
	int ShaderID = 0;
	enum ShaderType Type;

	void Compile();

	Shader(path ShaderFileName, ShaderType Type)
	{
		this->Name = ShaderFileName.string();
		this->SourceCode = FileUtils::GetInstance().ReadShader(ShaderFileName.string());
		bool Compiled = false;
		this->Type = Type;
		Compile();
	};

public:

	std::string GetName()
	{
		return Name;
	}
	int GetShaderID()
	{
		return ShaderID;
	}
};

class ShaderProgram
{
	friend class ShaderFactory;

	int ID = 0;
	ShaderProgram(std::shared_ptr<Shader> VS, std::shared_ptr<Shader> FS);

public:

	int GetShaderProgramID()
	{
		return ID;
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetVec3(const std::string& name, const glm::vec3& vec) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
	}

	void Use() const
	{
		glUseProgram(ID);
	}
};

class ShaderFactory {

	ShaderFactory() = default;
	ShaderFactory(const ShaderFactory&) = delete;
	ShaderFactory& operator=(const ShaderFactory&) = delete;

	std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderStorage;
	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> ShaderProgramStorage;

	std::shared_ptr<Shader> FindShader(const std::string& Name)
	{
		auto it = ShaderStorage.find(Name);
		return (it != ShaderStorage.end()) ? it->second : nullptr;
	}

	std::shared_ptr<ShaderProgram> FindShaderProgramByName(const std::string& Name)
	{
		auto it = ShaderProgramStorage.find(Name);
		return (it != ShaderProgramStorage.end()) ? it->second : nullptr;
	}

public:

	static ShaderFactory& GetInstance() {
		static ShaderFactory instance;
		return instance;
	}

	std::shared_ptr<Shader> CreateShader(const path& ShaderFileName, ShaderType Type) {

		if (FindShader(ShaderFileName.string()) != nullptr) {
			return ShaderStorage[ShaderFileName.string()];
		}

		auto shader = std::shared_ptr<Shader>(new Shader(ShaderFileName, Type));
		ShaderStorage[ShaderFileName.string()] = shader;
		return shader;
	}

	std::shared_ptr<ShaderProgram> CreateShaderProgram(std::shared_ptr<Shader> VS, std::shared_ptr<Shader> FS)
	{
		std::string ProgramName = VS->GetName() + FS->GetName();
		if (FindShaderProgramByName(ProgramName) != nullptr){
			return ShaderProgramStorage[ProgramName];
		}	

		auto ShaderPrg = std::shared_ptr<ShaderProgram>(new ShaderProgram(VS, FS));
		ShaderProgramStorage[ProgramName] = ShaderPrg;
		return ShaderPrg;
	}

	std::shared_ptr<ShaderProgram> FindShaderProgram(const std::string& name) {
		return FindShaderProgramByName(name);
	}

	std::shared_ptr<Shader> FindShaderByName(const std::string& name) {
		return FindShader(name);
	}

	std::shared_ptr<ShaderProgram> CreateShaderProgram(const path& VertexShaderFile, const path& FragmentShaderFile)
	{
		auto VertPtr = CreateShader(VertexShaderFile, VERTEX);
		auto FragPtr = CreateShader(FragmentShaderFile, FRAGMENT);
		return CreateShaderProgram(VertPtr, FragPtr);
	}
};
