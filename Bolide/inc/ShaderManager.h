#pragma once
#include "Shader.h"

//#include <vector>
//#include <tuple>
//#include <string>
//#include <memory>
//#include <algorithm>

using namespace std;

// Responsible for shaders management - creates shaders, inserts them into shaders vector, returns pointer of chosen shader

class ShaderManager
{
	vector < tuple<string, string, shared_ptr<Shader>>> shaders;

public:
	static ShaderManager& instance() {
		static ShaderManager manager;
		return manager;
	}

	// Creates shader and returns shader's pointer
	shared_ptr<Shader> getShader(string vertexName, string fragmentName)
	{
		auto shader = find_if(shaders.begin(), shaders.end(), [&](auto item)
			{
				auto vertex = get<0>(item);
				auto fragment = get<1>(item);
				return vertex == vertexName && fragment == fragmentName;
			}
		);

		if (shader != shaders.end()) {
			return get<2>(*shader);
		}

		auto newShader = make_shared<Shader>(vertexName.c_str(), fragmentName.c_str());
		shaders.emplace_back(make_tuple(vertexName, fragmentName, newShader));
		return newShader;
	}
};