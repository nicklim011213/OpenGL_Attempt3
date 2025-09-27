#pragma once

#include "GLFW/glfw3.h"

class FrameTimer
{
	FrameTimer() {};
	FrameTimer(const FrameTimer&) = delete;
	FrameTimer(operator =)(FrameTimer) = delete;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

public:

	static FrameTimer& GetInstance()
	{
		static FrameTimer FrameTime;
		return FrameTime;
	}

	float FrameTimerUpdate()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		return deltaTime;
	}

	float DeltaTime() const
	{
		return deltaTime;
	}
};