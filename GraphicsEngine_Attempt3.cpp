#include "Window.h"
#include "EngineContext.h"
#include "Shader.h"

#include "glm/vec4.hpp"
#include <iostream>

int main()
{
	Window::GetInstance().Init();
	Window::GetInstance().GenerateWindow(800, 600, "Graphics Engine Attempt 3");

	EngineContext::GetInstance().Initlize();
	EngineContext::GetInstance().SetBackgroundColor(1.0f, 1.0f, 1.0f);

	ShaderFactory::GetInstance().CreateFragmentShader("FragmentShaderBasic", "VertColorTex.fs");
	ShaderFactory::GetInstance().CreateVertexShader("VertexShaderBasic", "VertColorTex.vs");
	ShaderFactory::GetInstance().CreateShaderProgram("PhongLighting", ShaderFactory::GetInstance().FindVertexShader("VertexShaderBasic"), ShaderFactory::GetInstance().FindFragmentShader("FragmentShaderBasic"));



	while (!glfwWindowShouldClose(Window::GetInstance().GetWindow()))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(Window::GetInstance().GetWindow());
		glfwPollEvents();
	}
}

