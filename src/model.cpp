#include "model.h"
#include "textures.h"
#include "Log.h"

INCLUDE_LOGGER

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		logger.errMsg(FILE_READ_FAILURE, "Failed to read file while reading aiScene object");
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	logger.infoMsg("Successfully loaded a model");
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<vertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		vertex vert;
		glm::vec3 vector;
		glm::vec2 uv;
		//Processing vertex positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vert.position = vector;
		//Processing vertex normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vert.normal = vector;
		//Processing texture coordinates
		if (mesh->mTextureCoords[0])
		{
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vert.texcoords = uv;
		}
		else
			vert.texcoords = glm::vec2(0.0f);

		vertices.push_back(vert);
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indicies.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<texture> emissionMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emission");
		textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
	}

	return Mesh(vertices, indicies, textures);
}

std::vector<texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		for (int j = 0; j < loaded_textures.size(); j++)
		{
			if (std::strcmp(loaded_textures[j].path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(loaded_textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			texture tex;
			tex.id = TextureFromFile(str.C_Str(), directory);
			tex.type = typeName;
			tex.path = str.C_Str();
			textures.push_back(tex);
			loaded_textures.push_back(tex);
		}
	}
	return textures;
}

void Model::Draw(Shader& shader)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::unloadModel()
{
	if (!directory.empty())
	{
		meshes.clear();
		std::vector<Mesh>().swap(meshes);
		loaded_textures.clear();
		std::vector<texture>().swap(loaded_textures);
	}
}