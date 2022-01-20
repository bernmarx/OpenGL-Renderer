/*
	Model consists of several meshes and it is initialized
	from a 3D model file (this program is tuned for .obj but
	theoretically it can work with other extensions as well)

	Model class uses Assimp library for importing models
*/
#pragma once
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<texture> loaded_textures;

	void loadModel(std::string path);
	void unloadModel();
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typename);
public:
	unsigned int ID;
	Model(const char* path)
	{
		loadModel(path);
		ID = int(glfwGetTime() * 100.0f);
	}
	~Model()
	{
	}
	Model()
	{
		directory = nullptr;
	}
	bool operator==(Model& model)
	{
		return (this->ID == model.ID);
	}
	void Draw(Shader& shader);
};