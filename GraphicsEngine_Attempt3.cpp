#include "Window.h"
#include "EngineContext.h"
#include <iostream>

int main()
{
	Window::GetInstance().Init();
	Window::GetInstance().GenerateWindow(800, 600, "Graphics Engine Attempt 3");
	EngineContext::GetInstance().Initlize();

	while (!glfwWindowShouldClose(Window::GetInstance().GetWindow()))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(Window::GetInstance().GetWindow());
		glfwPollEvents();
	}
}

