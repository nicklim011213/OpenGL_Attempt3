#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
	Camera() {};
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 15.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
	glm::mat4 cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 Projection = glm::perspective(glm::radians(60.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
	
	float Speed = 2.5f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = 1920 / 2;
	float lastY = 1080 / 2;
	bool firstmouse = true;
	float yaw = -90.0f;
	float pitch = 0.0f;

	float DeltaTime() const
	{
		return deltaTime;
	}

	void Update()
	{
		cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}

public:

	static Camera& GetInstance() {
		static Camera instance;
		return instance;
	}

	void MoveCamera(glm::vec3 Direction)
	{
		cameraPos += glm::normalize(Direction) * (Speed * deltaTime);
		Update();
	}

	glm::mat4 GetView() const
	{
		return cameraView;
	}

	float FrameTimerUpdate()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		return deltaTime;
	}

	glm::vec3 GetPosition() const
	{
		return cameraPos;
	}

	void mousemovement(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstmouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstmouse = false;
		}

		float deltaX = xpos - lastX;
		float deltaY = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		float sens = 0.1f;
		pitch += sens * deltaY;
		yaw += sens * deltaX;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = direction;
		Update();
	}

	void Input(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		Camera& camera = Camera::GetInstance();
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.MoveCamera(cameraFront);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.MoveCamera(-cameraFront);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.MoveCamera(-cameraRight);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.MoveCamera(cameraRight);
	}

	glm::mat4 GetProjectionView()
	{
		return Projection;
	}

	void SetViewport(int width, int height)
	{
		Projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
	}
};