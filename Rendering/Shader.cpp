#include "Shader.h"

ShaderProgram::ShaderProgram(std::shared_ptr<Shader> VS, std::shared_ptr<Shader> FS)
{
	ID = glCreateProgram();
	glAttachShader(ID, VS->GetShaderID());
	glAttachShader(ID, FS->GetShaderID());
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

void Shader::Compile()
{
	if (Type == VERTEX)
	{
		ShaderID = glCreateShader(GL_VERTEX_SHADER);
	}
	else
	{
		ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	}
	const char* SourceCode = this->SourceCode.c_str();
	glShaderSource(ShaderID, 1, &SourceCode, NULL);
	glCompileShader(ShaderID);
	int success;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(ShaderID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}