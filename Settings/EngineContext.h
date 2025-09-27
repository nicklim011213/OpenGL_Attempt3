#pragma once

#include "glm/vec3.hpp"
#include <iostream>

class EngineContext {
	EngineContext() {};

	EngineContext(const EngineContext&) = delete;
	EngineContext& operator=(const EngineContext&) = delete;

public:
	static EngineContext& GetInstance() {
		static EngineContext instance;
		return instance;
	}

	void Initlize() {

		glewExperimental = GL_TRUE;
		glewInit();
		glEnable(GL_DEPTH_TEST);
	}
};
