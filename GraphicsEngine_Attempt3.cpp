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
	Window& window = Window::GetInstance();
	window.Init();
	window.GenerateWindow(800, 600, "Graphics Engine Attempt 3");

	EngineContext& context = EngineContext::GetInstance();
	context.Initlize();
	context.SetBackgroundColor(1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);

	ShaderFactory& Factory = ShaderFactory::GetInstance();
	std::shared_ptr<FragmentShader> FS = Factory.CreateFragmentShader("Tutfs", "ModelLoading.fs");
	std::shared_ptr<VertexShader> VS = Factory.CreateVertexShader("Tutvs", "ModelLoading.vs");
	std::shared_ptr<ShaderProgram> TutShader = Factory.CreateShaderProgram("TutPrg", VS, FS);

	Model model(FileUtils::GetInstance().GetModelPath() + "/backpack.obj");

	glm::vec3 viewpos = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 viewat = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 viewup = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 projection = glm::perspective(glm::radians(1.0f), (float)800 / (float)600, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(viewpos, viewat, viewup);
	glm::mat4 modelview = glm::mat4(1.0f);;
	modelview = glm::scale(modelview, glm::vec3(0.05f));

	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TutShader->Use();

		TutShader->SetMat4("projection", projection);
		TutShader->SetMat4("view", view);
		TutShader->SetMat4("model", modelview);
		TutShader->SetVec3("viewpos", glm::vec3(0.5f, 0.5f, 0.5f));

		model.Draw(TutShader);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}
}

