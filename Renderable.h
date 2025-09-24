#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "glm/glm.hpp"
#include "GL/glew.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "FileUtils.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;
	std::vector<unsigned int> TextureIDs;

	unsigned int VAO = 0, VBO = 0, EBO = 0;

public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures)
		: vertices(vertices), indices(indices), textures(textures)
	{
		Init();
	}

	void Draw(std::shared_ptr<ShaderProgram> Shader);

	void Init();
};

class Model
{
	void Load(std::string ModelObjectPath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(ModelObjectPath, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		Dir = ModelObjectPath.substr(0, ModelObjectPath.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(BuildMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh BuildMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:

	std::string Dir;
	std::vector<Mesh> meshes;
	bool ExternalTexture = false;
	std::vector<std::string> ExtTextures;

	glm::mat4 ModelProperties = glm::mat4(1.0f);

	Model(std::string ModelObjectPath)
	{
		Load(ModelObjectPath);
	}

	Model(std::string ModelObjectPath, bool TextureSeperate, std::vector<std::string> TextureFiles)
	{
		ExternalTexture = true;
		ExtTextures = TextureFiles;
		Load(ModelObjectPath);
	}

	void Draw(std::shared_ptr<ShaderProgram> shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

	void Scale(glm::vec3 scale)
	{
		ModelProperties = glm::scale(ModelProperties, scale);
	}

	void Move(glm::vec3 movement)
	{
		ModelProperties = glm::translate(ModelProperties, movement);
	}

	glm::mat4 GetModelView()
	{
		return ModelProperties;
	}
};

