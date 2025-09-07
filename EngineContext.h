#pragma once

#include "glm/vec3.hpp"

class EngineContext {
	EngineContext() {};

	EngineContext(const EngineContext&) = delete;
	EngineContext& operator=(const EngineContext&) = delete;

	glm::vec3 BackgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);

public:
	static EngineContext& GetInstance() {
		static EngineContext instance;
		return instance;
	}

	void Initlize() {
		glewExperimental = GL_TRUE;
		glewInit();
	}

	void SetBackgroundColor(float R, float G, float B)
	{
		BackgroundColor = glm::vec3(R,G,B);
	}

	glm::vec3 GetBackgroundColor()
	{
		return BackgroundColor;
	}
};
