#pragma once

#include "../core/core.hpp"
#include "../camera/camera.hpp"
#include "../materials/material.hpp"
#include "../shapes/shape.hpp"

namespace rt {

	struct SceneData {
		std::shared_ptr<Camera> camera;
		std::unordered_map<u32, std::shared_ptr<Material>> materials;
		std::vector<std::shared_ptr<Shape>> objects;
	};

	class Scene {
	public:
		Scene() = delete;
		Scene(const SceneData& data);
		static std::shared_ptr<Scene> FromFile(const std::string& filename);

	public:
		const std::shared_ptr<Camera> camera;
		const std::unordered_map<u32, std::shared_ptr<Material>> materials;
		const std::vector<std::shared_ptr<Shape>> objects;
	};

} // namespace rt
