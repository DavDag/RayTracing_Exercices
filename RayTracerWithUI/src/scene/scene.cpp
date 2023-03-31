#include "scene.hpp"

#include <fstream>
#include <iostream>

namespace rt {

	Scene::Scene(const SceneData& data) {
		this->w = 1920;
		this->h = 1080;
	}

	std::unique_ptr<Scene> Scene::FromFile(const std::string& filename) {
		std::ifstream in(filename);
		if (!in.is_open()) {
			std::cerr << "Unable to open scene file:" << filename << "\n";
			return std::unique_ptr<Scene>(nullptr);
		}
		return std::make_unique<Scene>(SceneData{});
	}

} // namespace rt
