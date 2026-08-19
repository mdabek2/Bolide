#pragma once

#include <gl/glew.h>
#include <vector>
#include <string>
#include "Object.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "Texture.h"

class Primitive : public Object
{
	virtual void generateVertices() = 0;
	virtual void generateIndices() = 0;

protected:

	glm::vec3 coordinates;
	glm::vec3 rotations;
	glm::vec3 size;
	glm::mat4 model;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	
	GLuint VAO, VBO, EBO;

	Texture texture;
	const std::string texturePath;

	void setUpBuffers()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
			
		glBindVertexArray(VAO);
	
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindVertexArray(0); 
	}

	void init()
	{
		generateVertices();
		generateIndices();
		setUpBuffers();
		texture = Texture(texturePath);
	}

public:
	Primitive(glm::vec3 coordinates, glm::vec3 rotations, glm::vec3 size = {1.f, 1.f, 1.f}, std::string texturePath = "textures/octopus.png") :
		coordinates(coordinates), rotations(rotations), size(size), texturePath(texturePath){}


	virtual ~Primitive()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}


	void draw(std::shared_ptr<Shader> shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, coordinates);
		model = glm::rotate(model, glm::radians(rotations.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotations.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotations.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size);

		texture.useTexture(shader);
		shader->setTransformMatrix("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void move(glm::vec3 displacement)
	{
		this->coordinates += displacement;
	}

	void scale(glm::vec3 amount)
	{
		this->size += amount;
	}

	void rotate(const glm::vec3& rotations)
	{
		this->rotations += rotations;
	}
};
