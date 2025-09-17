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

	/*
	Set material.diffuse and material.specular to texture units 0 and 1.

	Set material.shininess to something like 32.0.
	
	Set up a simple Light and viewPos uniform.
	
	Use the usual LearnOpenGL model loader code to bind textures and draw meshes.*/

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

		// View/Projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(1.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetMat4("projection", projection);
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetMat4("view", view);

		// Model Matrix
		glm::mat4 modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelview = glm::scale(modelview, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down

		// Material Properties
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetMat4("model", modelview);
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetInt("material.diffuse", 0);
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetInt("material.specular", 1);
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetInt("material.shininess", 32);

		// Light Properties
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetVec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));

		ShaderFactory::GetInstance().FindShaderProgram("TutPrg")->SetVec3("viewpos", glm::vec3(0.5f, 0.5f, 0.5f));

		model.Draw(ShaderFactory::GetInstance().FindShaderProgram("TutPrg"));

		glfwSwapBuffers(Window::GetInstance().GetWindow());
		glfwPollEvents();
	}
}

