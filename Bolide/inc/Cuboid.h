#pragma once
#include <iostream>
#include <GL/glew.h>
#include "ShaderManager.h"
#include "Primitive.h"

class Cuboid : public Primitive
{
private:
	void insertVert(glm::vec3 v, glm::vec3 n)
	{
		vertices.push_back(v.x);
		vertices.push_back(v.y);
		vertices.push_back(v.z);
		vertices.push_back(n.x);
		vertices.push_back(n.y);
		vertices.push_back(n.z);	
	}

	void generateWall(glm::vec3 normal, int num)
	{
		glm::vec3 firstPoint;
		firstPoint.x = normal.x == 0 ? -1 : normal.x;
		firstPoint.y = normal.y == 0 ? -1 : normal.y;
		firstPoint.z = normal.z == 0 ? 1 : normal.z;

		if (num == 0 || num == 2)
		{
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
		else
		{
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
			insertVert(firstPoint, normal);
			firstPoint = cross(firstPoint, normal) + normal;
			vertices.push_back(1.0f);
			vertices.push_back(1.0f);
		}
		
	}
	void generateVertices() override
	{
		generateWall({ -1, 0, 0 }, 0);
		generateWall({ 0, -1, 0 }, 1);
		generateWall({ 0, 0, -1 }, 2);
		generateWall({ 1, 0, 0 }, 3);
		generateWall({ 0, 1, 0 }, 4);
		generateWall({ 0, 0, 1 }, 5);
	}

	void generateIndices() override
	{
		for (int i = 0; i < 24; i += 4)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			indices.push_back(i + 2);
			indices.push_back(i + 3);
			indices.push_back(i);
		}
	}

public:
	Cuboid(glm::vec3 coordinates, glm::vec3 size,	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), std::string texturePath = "textures/octopus.png") :
		Primitive(coordinates, rotation, size, texturePath)
	{
		init();
	}
};