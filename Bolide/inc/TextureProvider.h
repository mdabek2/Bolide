#pragma once
#include <soil2/SOIL2.h>
#include <soil2/stb_image.h>
#include <unordered_map>
#include <string>
#include <tuple>


//Obs³uguje wektor tekstur
class TextureProvider {
	std::unordered_map<std::string, std::tuple<unsigned char*, int, int>> textures;
public:
	static TextureProvider& instance() {
		static TextureProvider provider;
		return provider;
	}

	//zwraca wskaŸnik do tekstury, wysokoœc i szerokoœæ na podstawie œcie¿ki do niej
	auto getTexture(const std::string& texturePath) {
		if (textures.find(texturePath) == textures.end())
		{
			int width, height, channels;
			const auto image = stbi_load(texturePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			if (image == nullptr)
				throw std::exception(std::string("Failed to load texture file " + texturePath).c_str());

			textures[texturePath] = std::make_tuple(image, width, height);
		}


		return textures[texturePath];
	}

	//czyœci pamiêæ o teksturach po ich na³o¿eniu
	void flushTextures()
	{
		for (auto& texture : textures)
		{
			const auto texturePointer = std::get<0>(texture.second);
			stbi_image_free(texturePointer);
		}
		textures.clear();
	}
};
