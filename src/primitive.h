/*
	These classes are supposed to represent primitives
	but I haven't implemented them properly yet
	so they are not used
*/
#pragma once
#include "mesh.h"

class Primitive : Mesh {
private:
	Mesh primitive;
public:
	Primitive(std::vector<vertex> verts, std::vector<unsigned int> inds, std::vector<texture> texts);
	Primitive();
	virtual void Draw(Shader& shader);
};

//Origin position is at the center of the bottom face of a cube
class Cube : Primitive {
private:
	Primitive prim;
	unsigned int ID;
	float side;
public:
	static const std::string texDirectory;
	Cube(float sd, const char* texPath = "");
	void Draw(Shader& shader);

	bool operator==(Cube& cube);
};