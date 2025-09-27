#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

#include "FileUtils.h"

class Shader
{
protected:
	Shader(std::string ShaderFileName)
	{
		this->Name = ShaderFileName;
		this->SourceCode = FileUtils::GetInstance().ReadShader(ShaderFileName);
	}
	bool Compiled = false;
	std::string Name;
	std::string SourceCode;
	int ShaderID = 0;

private:
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) = delete;
public:

	virtual ~Shader() = default;

	virtual void Compile() = 0;

	int GetShaderID()
	{
		return ShaderID;
	}

	std::string GetName()
	{
		return Name;
	}
};

class VertexShader : public Shader
{
	friend class ShaderFactory;

	VertexShader(const std::string& ShaderFileName) : Shader(ShaderFileName)
	{
		Compile();
	}

	void Compile() override;
};

class FragmentShader : public Shader
{
	friend class ShaderFactory;

	FragmentShader(const std::string& ShaderFileName) : Shader(ShaderFileName)
	{
		Compile();
	}

	void Compile() override;
};

class ShaderProgram
{
	friend class ShaderFactory;

	int ID = 0;

	ShaderProgram(std::shared_ptr<VertexShader> VertexShader, std::shared_ptr<FragmentShader> FragmentShader);

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
		return ShaderStorage.find(Name) != ShaderStorage.end() ? ShaderStorage[Name] : nullptr;
	}

public:

	static ShaderFactory& GetInstance() {
		static ShaderFactory instance;
		return instance;
	}

	std::shared_ptr<VertexShader> CreateVertexShader(const std::string& ShaderFileName) {

		if (ShaderStorage.find(ShaderFileName) != ShaderStorage.end()) {
			return std::static_pointer_cast<VertexShader>(ShaderStorage[ShaderFileName]);
		}

		auto shader = std::shared_ptr<VertexShader>(new VertexShader(ShaderFileName));
		ShaderStorage[ShaderFileName] = shader;
		return shader;
	}

	std::shared_ptr<FragmentShader> CreateFragmentShader(const std::string& ShaderFileName) {

		if (ShaderStorage.find(ShaderFileName) != ShaderStorage.end()) {
			return std::static_pointer_cast<FragmentShader>(ShaderStorage[ShaderFileName]);
		}

		auto shader = std::shared_ptr<FragmentShader>(new FragmentShader(ShaderFileName));
		ShaderStorage[ShaderFileName] = shader;
		return shader;
	}

	std::shared_ptr<ShaderProgram> CreateShaderProgram(std::shared_ptr<VertexShader> VertexShader, std::shared_ptr<FragmentShader> FragmentShader)
	{
		std::string ProgramName = VertexShader->GetName() + FragmentShader->GetName();
		if (ShaderProgramStorage.find(ProgramName) != ShaderProgramStorage.end())
		{
			return ShaderProgramStorage[ProgramName];
		}
		
		auto ShaderPrg = std::shared_ptr<ShaderProgram>(new ShaderProgram(VertexShader, FragmentShader));
		ShaderProgramStorage[ProgramName] = ShaderPrg;
		return ShaderPrg;
	}

	std::shared_ptr<VertexShader> FindVertexShader(const std::string& name) {
		return std::static_pointer_cast<VertexShader>(FindShader(name));
	}

	std::shared_ptr<FragmentShader> FindFragmentShader(const std::string& name) {
		return std::static_pointer_cast<FragmentShader>(FindShader(name));
	}

	std::shared_ptr<ShaderProgram> FindShaderProgram(const std::string& name) {
		if (ShaderProgramStorage.find(name) != ShaderProgramStorage.end())
		{
			return ShaderProgramStorage[name];
		}
		return nullptr;
	}

	std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& VertexShaderFile, const std::string& FragmentShaderFile)
	{
		auto VertPtr = CreateVertexShader(VertexShaderFile);
		auto FragPtr = CreateFragmentShader(FragmentShaderFile);
		return CreateShaderProgram(VertPtr, FragPtr);
	}
};
