#include "scene.h"
#include <gtc/matrix_transform.hpp>
#include <memory>

void Scene::addModel(Model& model, glm::vec3 loc)
{
	models.push_back({model, glm::translate(glm::mat4(1.0f), loc)});
}
void Scene::removeModel(Model& model)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (models[i].first == model)
		{
			models.erase(models.begin() + i);
		}
	}
	
}
//Not used
void Scene::addCube(glm::vec3 pos, float side, const char* tex)
{
	Cube cube = Cube(side, tex);
	cubes.push_back({ cube, glm::translate(glm::mat4(1.0f), pos) });
}

//Clean all data and stop drawing a scene
void Scene::unload()
{
	models.erase(models.begin(), models.end());
	models.shrink_to_fit();

	dirlights.erase(dirlights.begin(), dirlights.end());
	dirlights.shrink_to_fit();
	pntlights.erase(pntlights.begin(), pntlights.end());
	pntlights.shrink_to_fit();
	sptlights.erase(sptlights.begin(), sptlights.end());
	sptlights.shrink_to_fit();
}
void Scene::Draw(Shader& shader)
{
	updateLights(shader);
	for (auto model : models)
	{
		shader.set("model", model.second);
		shader.set("normalModel", glm::mat3(glm::inverse(glm::transpose(model.second))));
		(model.first).Draw(shader);
	}
	//Not used
	for (auto cube : cubes)
	{
		shader.set("model", cube.second);
		shader.set("normalModel", glm::mat3(glm::inverse(glm::transpose(cube.second))));
		(cube.first).Draw(shader);
	}
}
//Basically this function takes light parameters
//and sends them to shader as uniform variables
void Scene::updateLights(Shader& shader)
{
	for (int i = 0; i < dirlights.size(); i++)
	{
		std::string direction = "dirlight[" + std::to_string(i) + "].direction";
		std::string ambient = "dirlight[" + std::to_string(i) + "].ambient";
		std::string diffuse = "dirlight[" + std::to_string(i) + "].diffuse";
		std::string specular = "dirlight[" + std::to_string(i) + "].specular";
		shader.set(direction.c_str(), dirlights[i].direction);
		shader.set(ambient.c_str(), dirlights[i].ambient);
		shader.set(diffuse.c_str(), dirlights[i].diffuse);
		shader.set(specular.c_str(), dirlights[i].specular);
	}
	for (int i = 0; i < pntlights.size(); i++)
	{
		std::string position = "pntlight[" + std::to_string(i) + "].position";
		std::string constant = "pntlight[" + std::to_string(i) + "].constant";
		std::string linear = "pntlight[" + std::to_string(i) + "].linear";
		std::string quadratic = "pntlight[" + std::to_string(i) + "].quadratic";
		std::string ambient = "pntlight[" + std::to_string(i) + "].ambient";
		std::string diffuse = "pntlight[" + std::to_string(i) + "].diffuse";
		std::string specular = "pntlight[" + std::to_string(i) + "].specular";
		shader.set(position.c_str(), pntlights[i].position);
		shader.set(constant.c_str(), pntlights[i].constant);
		shader.set(linear.c_str(), pntlights[i].linear);
		shader.set(quadratic.c_str(), pntlights[i].quadratic);
		shader.set(ambient.c_str(), pntlights[i].ambient);
		shader.set(diffuse.c_str(), pntlights[i].diffuse);
		shader.set(specular.c_str(), pntlights[i].specular);
		
		//Not used. Meant to draw a cube in a position where point light is
		lightShader->use();
		std::unique_ptr<Cube> lightCube = std::make_unique<Cube>(0.1f, "white.png");
		lightShader->set("model", glm::translate(glm::mat4(1.0f), pntlights[i].position + glm::vec3(0.0f, -0.05f, 0.0f)));
		lightCube->Draw(*lightShader);
		shader.use();
	}
	for (int i = 0; i < sptlights.size(); i++)
	{
		std::string direction = "sptlight[" + std::to_string(i) + "].direction";
		std::string position = "sptlight[" + std::to_string(i) + "].position";
		std::string cutoff = "sptlight[" + std::to_string(i) + "].cutoff";
		std::string outerCutoff = "sptlight[" + std::to_string(i) + "].outercutoff";
		std::string ambient = "sptlight[" + std::to_string(i) + "].ambient";
		std::string diffuse = "sptlight[" + std::to_string(i) + "].diffuse";
		std::string specular = "sptlight[" + std::to_string(i) + "].specular";
		shader.set(direction.c_str(), sptlights[i].direction);
		shader.set(position.c_str(), sptlights[i].position);
		shader.set(cutoff.c_str(), sptlights[i].cutoff);
		shader.set(outerCutoff.c_str(), sptlights[i].outerCutoff);
		shader.set(ambient.c_str(), sptlights[i].ambient);
		shader.set(diffuse.c_str(), sptlights[i].diffuse);
		shader.set(specular.c_str(), sptlights[i].specular);
	}
}

void Scene::moveModel(Model& model, glm::vec3 move)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (model == models[i].first)
			models[i].second = glm::translate(models[i].second, move);
	}
}
void Scene::scaleModel(Model& model, glm::vec3 scl)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (model == models[i].first)
			models[i].second = glm::scale(models[i].second, scl);
	}
}
void Scene::rotateModel(Model& model, glm::vec3 ax, float dgrs)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (model == models[i].first)
			models[i].second = glm::rotate(models[i].second, glm::radians(dgrs), ax);
	}
}
void Scene::addDirectionalLight(DirLight& light)
{
	dirlights.push_back(light);
}
void Scene::addPointLight(PntLight& light)
{
	pntlights.push_back(light);
}
void Scene::addSpotLight(SptLight& light)
{
	sptlights.push_back(light);
}
void Scene::removeDirectionalLight(DirLight& light)
{
	for (int i = 0; i < dirlights.size(); i++)
	{
		if (dirlights[i] == light)
		{
			dirlights.erase(dirlights.begin() + i);
		}
	}
}
void Scene::removePointLight(PntLight& light)
{
	for (int i = 0; i < pntlights.size(); i++)
	{
		if (pntlights[i] == light)
		{
			pntlights.erase(pntlights.begin() + i);
		}
	}
}
void Scene::removeSpotLight(SptLight& light)
{
	for (int i = 0; i < sptlights.size(); i++)
	{
		if (sptlights[i] == light)
		{
			sptlights.erase(sptlights.begin() + i);
		}
	}
}

//Not used
void Scene::addLightShader(Shader* shader)
{
	lightShader = shader;
}