#pragma once
#include <string>
#include <memory>
#include "Shader.h"
#include "TextureProvider.h"


class Texture {
	int width;
	int height;
	std::string textureName;
	GLuint texture;

	void setActiveTexture() const
	{
		glActiveTexture(GL_TEXTURE0);
	}
public:

	Texture() {}

	 
	Texture(const std::string& texturePath, std::string textureName = "Texture0")
		: textureName(textureName) {
		std::cout << texturePath;
 		glGenTextures(1, &texture);
		//Ustawia parametry nak³adania i zawijania tekstury
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//ustawia parametry filtrowania tekstury
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// dodaje teksturê do wektora tekstur, œci¹ga wskaŸnik do niej, jej szerokoœæ i wysokoœæ
		unsigned char* image;
		std::tie(image, width, height) = TextureProvider::instance().getTexture(texturePath);

		setActiveTexture();
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	int getWidth() const
	{
		return width;
	}
	int getHeight() const
	{
		return height;
	}

	void useTexture(std::shared_ptr<Shader> shader) const
	{
		shader->use();
		setActiveTexture();
		glBindTexture(GL_TEXTURE_2D, texture);
		//shader->setInt(textureName, 0);
	}
};

