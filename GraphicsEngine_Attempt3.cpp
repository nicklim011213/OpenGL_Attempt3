#define STB_IMAGE_IMPLEMENTATION

#include "Window.h"
#include "EngineContext.h"
#include "Shader.h"
#include "Renderable.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mousemovement(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

int main()
{
	Window& window = Window::GetInstance();
	window.Init();
	window.GenerateWindow(1920, 1080, "Graphics Engine Attempt 3");
	glfwSetFramebufferSizeCallback(window.GetWindow(), framebuffer_size_callback);
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window.GetWindow(), mousemovement);

	EngineContext& context = EngineContext::GetInstance();
	context.Initlize();

	ShaderFactory& Factory = ShaderFactory::GetInstance();
	std::shared_ptr<ShaderProgram> TutShader = Factory.CreateShaderProgram("ModelLoading.vs", "ModelLoading.fs");

	Camera& camera = Camera::GetInstance();

	std::vector<std::string> TextureFileNames = { FileUtils::GetInstance().GetTexturePath() + "\\backpack_diffuse.jpg" };
	Model model(FileUtils::GetInstance().GetModelPath() + "\\backpack.obj", true, TextureFileNames);
	model.Move(glm::vec3(0.0f, 0.0f, 8.0f));
	model.Scale(glm::vec3(0.2f, 0.2f, 0.2f));

	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TutShader->Use();
		TutShader->SetMat4("projection", camera.GetProjectionView());
		TutShader->SetMat4("view", camera.GetView());
		TutShader->SetMat4("model", model.GetModelView());
		TutShader->SetVec3("viewpos", camera.GetPosition());

		model.Draw(TutShader);

		processInput(window.GetWindow());
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
		camera.FrameTimerUpdate();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Camera& camera = Camera::GetInstance();
	camera.SetViewport(width, height);
}

void mousemovement(GLFWwindow* window, double xpos, double ypos)
{
	Camera& camera = Camera::GetInstance();
	camera.mousemovement(window, xpos, ypos);
}

void processInput(GLFWwindow* window)
{
	Camera& camera = Camera::GetInstance();
	camera.Input(window);
}
