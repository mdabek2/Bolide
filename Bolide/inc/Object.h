#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

// Represents every created object, and basic operations and tranformations.
class Object {
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaleValue;
public:
	Object() : position(0.0f), rotation(0.0f), scaleValue(1.0f) {}
	virtual ~Object() = default;

	virtual void draw(std::shared_ptr<Shader>) {};
	virtual void move(glm::vec3 displacement) {
		position += displacement;
	};
	virtual void scale(glm::vec3 amount) {
		scaleValue *= amount;
	};
	virtual void rotate(const glm::vec3& amount) {
		rotation += amount;
	};
	glm::vec3 getPosition() const {
		return position; 
	} 
	glm::vec3 getRotation() const { 
		return rotation; 
	} 
	glm::vec3 getScale() const { 
		return scaleValue; 
	}
	glm::mat4 getModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scaleValue);

		return model;
    }
};