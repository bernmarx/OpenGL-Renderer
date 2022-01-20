/*
	This contains definitions of light structures
	which were made for an easier use of lighting.
	This application implenets Phong shading model so
	all lights have ambient, diffuse and specular component
*/
#pragma once
#include <glm.hpp>

//Basic class
struct Light {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//Specify each light component individually
	Light(glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) : ambient(amb), diffuse(dif), specular(spec)
	{}
};
//Directional light
struct DirLight : Light {
	glm::vec3 direction;

	DirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) : Light(amb, dif, spec), direction(glm::normalize(dir))
	{}

	bool operator==(DirLight& light)
	{
		return ((this->direction == light.direction) && (this->ambient == light.ambient) && (this->diffuse == light.diffuse)
				&& (this->specular == light.specular));
	}
};
//Point light
struct PntLight : Light {
	glm::vec3 position;

	//Parameters used for calculating attenuation
	float constant;
	float linear;
	float quadratic;

	PntLight(glm::vec3 pos, float c, float l, float q, 
			 glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) : Light(amb, dif, spec), position(pos), constant(c), linear(l), quadratic(q)
	{}

	bool operator==(PntLight& light)
	{
		return ((this->position == light.position) && (this->ambient == light.ambient) && (this->diffuse == light.diffuse)
			&& (this->specular == light.specular) && (this->constant == light.constant && (this->linear == light.constant)
			&& (this->quadratic == light.quadratic)));
	}
};
//Spotlight
struct SptLight : Light {
	glm::vec3 position;
	glm::vec3 direction;

	float cutoff;
	float outerCutoff;

	SptLight(glm::vec3 pos, glm::vec3 dir, float cf, float ocf, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec) : Light(amb, dif, spec), 
			 position(pos), direction(glm::normalize(dir)), cutoff(cos(glm::radians(cf))), outerCutoff(cos(glm::radians(ocf)))
	{}

	bool operator==(SptLight& light)
	{
		return ((this->position == light.position) && (this->ambient == light.ambient) && (this->diffuse == light.diffuse)
			&& (this->specular == light.specular) && (this->cutoff == light.cutoff && (this->outerCutoff == light.outerCutoff)
				&& (this->direction == light.direction)));
	}
};