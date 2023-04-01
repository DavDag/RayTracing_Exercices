#pragma once

#include "../core/core.hpp"
#include "../scene/scene.hpp"
#include "../options/options.hpp"
#include "../image/image.hpp"

namespace rt {

	class RayTracer {
	public:
		RayTracer(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Options>& options, Image& image);

		void build();
		void process();

	private:
		Pixel pixel(i32 x, i32 y);

	private:
		const std::shared_ptr<Scene> _scene;
		const std::shared_ptr<Options> _options;
		Image& _image;
	};

} // namespace rt
