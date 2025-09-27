#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Window {
	Window() {};
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	GLFWwindow* window = nullptr;

public:

	static Window& GetInstance() {
		static Window instance;
		return instance;
	}

	void Init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	}

	void GenerateWindow(int Width, int Height, std::string WindowTitle) {
		window = glfwCreateWindow(Width, Height, WindowTitle.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glViewport(0, 0, Width, Height);
	}

	void ConfigureWindowCallbacks(GLFWcursorposfun CursorCallback, GLFWframebuffersizefun FrameBufferCallback) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetFramebufferSizeCallback(window, FrameBufferCallback);
		glfwSetCursorPosCallback(window, CursorCallback);
	}

	GLFWwindow* GetWindow() const {
		return window;
	}

	void PollEvents()
	{
		glfwPollEvents();
	}

	void SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	bool IsWindowClosed()
	{
		return glfwWindowShouldClose(window);
	}
};

