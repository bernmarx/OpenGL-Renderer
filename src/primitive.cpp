#include "primitive.h"
#include "textures.h"

Primitive::Primitive(std::vector<vertex> verts, std::vector<unsigned int> inds, std::vector<texture> texts)
{
	primitive = Mesh(verts, inds, texts);
	glfwGetTime();
}
Primitive::Primitive()
{

}
void Primitive::Draw(Shader& shader)
{
	primitive.Draw(shader);
}

const std::string Cube::texDirectory = "resources/cubes";
Cube::Cube(float sd, const char* texPath)
{
	ID = (int)glfwGetTime() % 255;
	side = sd;

	std::vector<vertex> verts;
	std::vector<unsigned int> inds;
	std::vector<texture> texts;
	float halfside = sd / 2.0f;

	verts.push_back(vertex(glm::vec3(-halfside, 0, halfside), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	verts.push_back(vertex(glm::vec3(halfside, 0,  halfside), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	verts.push_back(vertex(glm::vec3(-halfside, 0, -halfside), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)));
	verts.push_back(vertex(glm::vec3(halfside, 0, -halfside), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f)));

	verts.push_back(vertex(glm::vec3(-halfside, sd, halfside), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
	verts.push_back(vertex(glm::vec3(+halfside, sd, halfside), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
	verts.push_back(vertex(glm::vec3(-halfside, sd, -halfside), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f)));
	verts.push_back(vertex(glm::vec3(halfside, sd, -halfside), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)));

	//Bottom
	inds.push_back(0); inds.push_back(1); inds.push_back(2); inds.push_back(1); inds.push_back(2); inds.push_back(3);
	//Front
	inds.push_back(0); inds.push_back(2); inds.push_back(6); inds.push_back(0); inds.push_back(4); inds.push_back(6);
	//Left
	inds.push_back(2); inds.push_back(3); inds.push_back(6); inds.push_back(3); inds.push_back(7); inds.push_back(6);
	//Back
	inds.push_back(3); inds.push_back(7); inds.push_back(5); inds.push_back(5); inds.push_back(1); inds.push_back(3);
	//Right
	inds.push_back(0); inds.push_back(1); inds.push_back(4); inds.push_back(1); inds.push_back(4); inds.push_back(5);
	//Top
	inds.push_back(4); inds.push_back(5); inds.push_back(6); inds.push_back(5); inds.push_back(6); inds.push_back(7);

	texture tex;
	if (texPath != "")
	{
		tex.id = TextureFromFile(texPath, texDirectory);
		tex.path = texDirectory + '/' + texPath;
		tex.type = "texture_diffuse1";
	}
	texts.push_back(tex);

	prim = Primitive(verts, inds, texts);
}
bool Cube::operator==(Cube& cube)
{
	return (ID == cube.ID);
}

void Cube::Draw(Shader& shader)
{
	prim.Draw(shader);
}