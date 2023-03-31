#pragma once

#include "../core/core.hpp"

#include <string>
#include <memory>

namespace rt {

	struct SceneData {

	};

	class Scene {
	public:
		Scene() = delete;
		Scene(const SceneData& data);
		static std::unique_ptr<Scene> FromFile(const std::string& filename);

	public:
		i32 w, h;
	};

} // namespace rt
