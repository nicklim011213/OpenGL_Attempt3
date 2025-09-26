#include "Shader.h"

void FragmentShader::Compile()
{
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
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void VertexShader::Compile()
{
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

ShaderProgram::ShaderProgram(std::shared_ptr<VertexShader> VertexShader, std::shared_ptr<FragmentShader> FragmentShader)
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