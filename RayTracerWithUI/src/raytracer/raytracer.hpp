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
		void _process(i32 x, i32 y, i32 sampleIndex);
		Color _pixel(i32 x, i32 y);
		Color _trace(Ray& ray, i32 depth);

	private:
		std::shared_ptr<Scene> _scene;
		std::shared_ptr<Options> _options;
		Image& _image;
	};

} // namespace rt
