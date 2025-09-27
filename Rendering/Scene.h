#pragma once

#include <vector>
#include <iostream>

#include "Renderable.h"
#include "Shader.h"
#include "Camera.h"

class Scene
{
	std::vector<Model> SceneObjects;
	bool StaticsSet = false;

	std::shared_ptr<ShaderProgram> SceneShader = nullptr;
	Camera& SceneCamera;

	void SetupSceneStatics()
	{
		SceneShader->Use();
		SceneShader->SetVec3("lightPos", glm::vec3(0.0f, 0.0f, 9.0f));
		SceneShader->SetMat4("projection", SceneCamera.GetProjectionView());
	}

public: 

	Scene(std::shared_ptr<ShaderProgram> Shader)
		: SceneShader(Shader), SceneCamera(Camera::GetInstance())
	{

	}

	void AddObject(Model& model)
	{
		SceneObjects.push_back(model);
	}

	void RenderScene()
	{
		if (!StaticsSet)
		{
			SetupSceneStatics();
		}
		SceneShader->SetMat4("view", SceneCamera.GetView());
		SceneShader->SetVec3("viewPos", SceneCamera.GetPosition());
		for (Model object : SceneObjects)
		{
			SceneShader->SetMat4("model", object.GetModelView());
			object.Draw(SceneShader);
		}
	}

	float UpdateFrameTimer()
	{
		return SceneCamera.FrameTimerUpdate();
	}
};