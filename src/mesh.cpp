#include "mesh.h"

Mesh::Mesh(std::vector<vertex> _vertices, std::vector<unsigned int> _indicies, std::vector<texture> _textures)
{
	vertices = _vertices;
	indicies = _indicies;
	textures = _textures;

	setupMesh();
}
Mesh::Mesh()
{

}
Mesh::~Mesh()
{
	vertices.clear();
	std::vector<vertex>().swap(vertices);
	indicies.clear();
	std::vector<unsigned int>().swap(indicies);
	std::vector<texture>().swap(textures);
}

void Mesh::setupMesh()
{
	//Generate all necessary buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//Bind them to context and then send data to buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	//Vertex coordinates at layout = 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);
	//Vertex normal at layout = 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
	glEnableVertexAttribArray(1);
	//Texture coordinates at layout = 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texcoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNMB = 1;
	unsigned int specularNMB = 1;
	unsigned int emissionNMB = 1;
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		//I use a convention by which all
		//texture maps are named texture_<type><number>
		//where <type> is a texture's type (diffuse, specular, emissive etc.)
		//and <number> is a number of a texture map (from 1 to N)
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNMB++);
		else if (name == "texture_specular")
			number = std::to_string(specularNMB++);
		else if (name == "texture_emission")
			number = std::to_string(emissionNMB++);

		shader.set(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}