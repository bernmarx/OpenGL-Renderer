/*
	Scene class represents, well, scenes, which
	I define as "collection of models" and allows
	to interact with these models by moving,
	scaling and rotating them
	Each scene contains different lights
	and models which are paired with
	their respective model matrix 

	For information on specific functions check out scene.cpp
*/
#pragma once
#include "model.h"
#include "light.h"
#include "primitive.h"
#include "shader.h"

class Scene {
private:
	std::vector<std::pair<Model, glm::mat4>> models;
	std::vector <std::pair<Cube, glm::mat4>> cubes;		//Not used
	std::vector<DirLight> dirlights;
	std::vector<PntLight> pntlights;
	std::vector<SptLight> sptlights;
	Shader* lightShader;
public:
	void Draw(Shader& shader);
	void updateLights(Shader& shader);
	void unload();

	void addModel(Model& model, glm::vec3 loc = glm::vec3(0.0f));
	void removeModel(Model& model);
	void moveModel(Model& model, glm::vec3 move);
	void scaleModel(Model& model, glm::vec3 scl);
	void rotateModel(Model& model, glm::vec3 ax, float dgrs);

	void addCube(glm::vec3 pos, float side, const char* tex = "");
	void removeCube(Cube* cube);

	void addDirectionalLight(DirLight& light);
	void removeDirectionalLight(DirLight& light);
	void addPointLight(PntLight& light);
	void removePointLight(PntLight& light);
	void addSpotLight(SptLight& light);
	void removeSpotLight(SptLight& light);

	void addLightShader(Shader* shader);
};