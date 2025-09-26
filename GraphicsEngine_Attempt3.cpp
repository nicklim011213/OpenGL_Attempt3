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
	std::shared_ptr<ShaderProgram> TutShader = Factory.CreateShaderProgram("ModelLoading.vs", "ModelLoading_Advanced.fs");

	Camera& camera = Camera::GetInstance();

	FileUtils& FileSystem = FileUtils::GetInstance();
	std::string ModelPath = FileSystem.GetModelPath();
	std::string TexturePath = FileSystem.GetTexturePath();
	std::vector<std::string> TextureFileNames = { TexturePath + "\\backpack_diffuse.jpg", TexturePath + "\\backpack_specular.jpg"};

	Model model(FileUtils::GetInstance().GetModelPath() + "\\backpack.obj", true, TextureFileNames);
	model.Move(glm::vec3(0.0f, 0.0f, 8.0f));
	model.Scale(glm::vec3(0.2f, 0.2f, 0.2f));

	TutShader->Use();
	TutShader->SetVec3("lightPos", glm::vec3(0.0f, 0.0f, 9.0f));
	TutShader->SetMat4("projection", camera.GetProjectionView());
	TutShader->SetMat4("model", model.GetModelView());

	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TutShader->SetMat4("view", camera.GetView());
		TutShader->SetVec3("viewPos", camera.GetPosition());

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
