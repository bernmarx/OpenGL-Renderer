/*
	Each mesh contains all necessary data to draw a mesh (duh)
	This includes vertex, normal and texture coordinates as well as
	face indicies
*/
#pragma once
#include <glm.hpp>
#include <string>
#include <vector>
#include "shader.h"

struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
	vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex) : position(pos), normal(norm), texcoords(tex)
	{}
	vertex()
	{}
};
struct texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<vertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<texture> textures;

	Mesh(std::vector<vertex> _vertices, std::vector<unsigned int> _indicies, std::vector<texture> _textures);
	Mesh();
	~Mesh();
	void Draw(Shader& shader);
private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};