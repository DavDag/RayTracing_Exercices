#include "raytracer.hpp"

namespace rt {

	RayTracer::RayTracer(
		const std::shared_ptr<Scene>& scene,
		const std::shared_ptr<Options>& options,
		Image& image
	):
		_scene(scene), _options(options), _image(image)
	{

	}

	void RayTracer::build() {

	}

	void RayTracer::process() {
		i32 w = this->_options->w;
		i32 h = this->_options->h;
		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < w; ++x) {
				Pixel p = this->pixel(x, y);
				this->_image.set(x, y, p);
			}
		}
	}

	Pixel RayTracer::pixel(i32 px, i32 py) {
		Camera& camera = *this->_scene->camera;
		i32 w = this->_options->w;
		i32 h = this->_options->h;
		i32 samples = this->_options->samples;
		i32 maxdepth = this->_options->maxdepth;
		Pixel out(0.0f);
		//for (int si = 0; si < samples; ++si) {
			f32 dx = (px + 0.0f) / w;
			f32 dy = (py + 0.0f) / h;
			Ray ray = camera.getRay(dx, dy);
			out.r = ray.dir.y / 2.0f + 0.5f;
		//}
		return out;
	}

} // namespace rt
