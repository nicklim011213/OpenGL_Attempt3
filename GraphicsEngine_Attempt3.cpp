#define STB_IMAGE_IMPLEMENTATION

#include "Window.h"
#include "EngineContext.h"
#include "Shader.h"
#include "Renderable.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main()
{
	Window::GetInstance().Init();
	Window::GetInstance().GenerateWindow(800, 600, "Graphics Engine Attempt 3");

	EngineContext::GetInstance().Initlize();
	EngineContext::GetInstance().SetBackgroundColor(1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	ShaderFactory::GetInstance().CreateFragmentShader("Tutfs", "Tutorial.fs");
	ShaderFactory::GetInstance().CreateVertexShader("Tutvs", "Tutorial.vs");
	ShaderFactory::GetInstance().CreateShaderProgram("TutPrg", ShaderFactory::GetInstance().FindVertexShader("Tutvs"), ShaderFactory::GetInstance().FindFragmentShader("Tutfs"));

	Model model(FileUtils::GetInstance().GetModelPath() + "/backpack.obj");
	

	while (!glfwWindowShouldClose(Window::GetInstance().GetWindow()))
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->Use();
		glm::mat4 projection = glm::perspective(glm::radians(1.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetMat4("projection", projection);
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetMat4("view", view);

		glm::mat4 modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelview = glm::scale(modelview, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetMat4("model", modelview);
		model.Draw(ShaderFactory::GetInstance().FindShaderProgram("TutPrg"));

		glfwSwapBuffers(Window::GetInstance().GetWindow());
		glfwPollEvents();
	}
}

