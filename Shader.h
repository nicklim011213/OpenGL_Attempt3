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
	Shader(std::string ShaderName, std::string ShaderFileName)
	{
		this->Name = ShaderName;
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
};

class VertexShader : public Shader
{
	friend class ShaderFactory;

	VertexShader(const std::string& name, const std::string& ShaderFileName) : Shader(name, ShaderFileName)
	{
		Compile();
	}

	void Compile() override {
		Compiled = true;
		ShaderID = glCreateShader(GL_VERTEX_SHADER);
		const char* SourceCode = this->SourceCode.c_str();
		glShaderSource(ShaderID, 1, &SourceCode, NULL);
		glCompileShader(ShaderID);
		int success;
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(ShaderID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
};

class FragmentShader : public Shader
{
	friend class ShaderFactory;

	FragmentShader(const std::string& name, const std::string& ShaderFileName) : Shader(name, ShaderFileName)
	{
		
	}

	void Compile() override {
		Compiled = true;
		ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* SourceCode = this->SourceCode.c_str();
		glShaderSource(ShaderID, 1, &SourceCode, NULL);
		glCompileShader(ShaderID);
		int success;
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(ShaderID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VFRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
};

class ShaderProgram
{
	friend class ShaderFactory;

	int ID = 0;

	ShaderProgram(std::shared_ptr<VertexShader> VertexShader, std::shared_ptr<FragmentShader> FragmentShader)
	{
		ID = glCreateProgram();
		glAttachShader(ID, VertexShader->GetShaderID());
		glAttachShader(ID, FragmentShader->GetShaderID());
		glLinkProgram(ID);

		int success;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

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

	std::shared_ptr<VertexShader> CreateVertexShader(const std::string& name, const std::string& ShaderName) {

		if (ShaderStorage.find(name) != ShaderStorage.end()) {
			return std::static_pointer_cast<VertexShader>(ShaderStorage[name]);
		}

		auto shader = std::shared_ptr<VertexShader>(new VertexShader(name, ShaderName));
		ShaderStorage[name] = shader;
		return shader;
	}

	std::shared_ptr<FragmentShader> CreateFragmentShader(const std::string& name, const std::string& ShaderName) {

		if (ShaderStorage.find(name) != ShaderStorage.end()) {
			return std::static_pointer_cast<FragmentShader>(ShaderStorage[name]);
		}

		auto shader = std::shared_ptr<FragmentShader>(new FragmentShader(name, ShaderName));
		ShaderStorage[name] = shader;
		return shader;
	}

	std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& name, std::shared_ptr<VertexShader> VertexShader, std::shared_ptr<FragmentShader> FragmentShader)
	{
		if (ShaderProgramStorage.find(name) != ShaderProgramStorage.end())
		{
			return ShaderProgramStorage[name];
		}
		
		auto ShaderPrg = std::shared_ptr<ShaderProgram>(new ShaderProgram(VertexShader, FragmentShader));
		ShaderProgramStorage[name] = ShaderPrg;
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
};
