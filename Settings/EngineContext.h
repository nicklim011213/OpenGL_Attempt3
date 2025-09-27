#pragma once

#include "GL/glew.h"
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

	void Initialize() {

		glewExperimental = GL_TRUE;
		glewInit();
		glEnable(GL_DEPTH_TEST);
	}
};
