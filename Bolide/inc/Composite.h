#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Object.h"
#include "ShaderManager.h"

class Composite : public Object {
public:
	void draw(std::shared_ptr<Shader> shader) override
	{
		for (auto& child : children)
			child->draw(shader);
	}

	void move(glm::vec3 displacement) override
	{
		for (auto& child : children)
			child->move(displacement);
	}

	void addChild(unique_ptr<Object>&& child) {
		children.push_back(std::move(child));
	}

	virtual ~Composite() = default;

protected:
	std::vector<unique_ptr<Object>> children;
};