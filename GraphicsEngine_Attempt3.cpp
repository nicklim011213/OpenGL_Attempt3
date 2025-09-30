#define STB_IMAGE_IMPLEMENTATION

#include "Window.h"
#include "EngineContext.h"
#include "Shader.h"
#include "Renderable.h"
#include "Camera.h"
#include "Scene.h"

#include <boost/filesystem.hpp>

#include <glm/glm.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
void OnKeyboardPress(Window& window);
Window& Init();

typedef boost::filesystem::path path;
typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;

int main()
{

	Window& window = Init();

	ShaderFactory& Factory = ShaderFactory::GetInstance();
	ShaderProgramPtr TutShader = Factory.CreateShaderProgram("Gouraud.vs", "Gouraud.fs");
	
	FileUtils& FileSystem = FileUtils::GetInstance();
	path ModelPath = FileSystem.GetModelPath();
	path TexturePath = FileSystem.GetTexturePath();
	
	std::vector<path> TextureFileNames = { 
		TexturePath / "backpack_diffuse.jpg", 
		TexturePath / "backpack_specular.jpg"
	};

	Model model(ModelPath / "backpack.obj", true, TextureFileNames);
	model.Move(glm::vec3(0.0f, 0.0f, 8.0f));
	model.Scale(glm::vec3(0.2f, 0.2f, 0.2f));

	Scene Scene1(TutShader);
	Scene1.AddObject(model);

	while (!window.IsWindowClosed()) {

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Scene1.RenderScene();

		OnKeyboardPress(window);
		window.PollEvents();
		window.SwapBuffers();
		FrameTimer::GetInstance().FrameTimerUpdate();
	}
}

Window& Init()
{
	Window& window = Window::GetInstance();
	window.Init();
	window.GenerateWindow(1920, 1080, "Graphics Engine Attempt 3");
	window.ConfigureWindowCallbacks(OnMouseMove, framebuffer_size_callback);

	EngineContext& context = EngineContext::GetInstance();
	context.Initialize();

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Camera& camera = Camera::GetInstance();
	camera.SetViewport(width, height);
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	Camera& camera = Camera::GetInstance();
	camera.OnMouseMove(window, xpos, ypos);
}

void OnKeyboardPress(Window& window)
{
	Camera& camera = Camera::GetInstance();
	camera.OnKeyboardPress(window);
}
