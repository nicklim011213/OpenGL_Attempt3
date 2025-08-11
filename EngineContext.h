#pragma once

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
		glewInit();
	}
};
