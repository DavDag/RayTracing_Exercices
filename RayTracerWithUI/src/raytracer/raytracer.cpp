#include "raytracer.hpp"

#include "rayhit.hpp"

#include <ppl.h>

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
		i32 w = this->_scene->camera->imgW();
		i32 h = this->_scene->camera->imgH();
		concurrency::parallel_for(0, h, [&](i32 y) {
			for (int x = 0; x < w; ++x) {
				Color p = this->pixel(x, y);
#ifdef _DEBUG
				if ((p.r > 1.0f || p.g > 1.0f || p.b > 1.0f)
					|| (p.r < 0.0f || p.g < 0.0f || p.b < 0.0f))
				{
					__debugbreak();
					exit(1);
				}
#endif // _DEBUG
				p = p.gammaCorrected(2.0f);
				this->_image.set(x, y, p);
			}
		});
	}

	Color RayTracer::pixel(i32 px, i32 py) {
		Camera& camera = *this->_scene->camera;
		i32 w = camera.imgW(), h = camera.imgH();
		i32 samples = this->_options->samples;
		i32 maxdepth = this->_options->maxdepth;
		Color out(0.0f);
		for (int si = 0; si < samples; ++si) {
			f32 dx = (px + rnd_uniform<f32>(0.0f, 1.0f)) / w;
			f32 dy = (py + rnd_uniform<f32>(0.0f, 1.0f)) / h;
			Ray ray = camera.getRay(dx, dy);
			out += this->trace(ray, maxdepth);
		}
		out /= (f32)samples;
		return out;
	}
	
	Color RayTracer::trace(Ray& ray, i32 depth) {
		if (depth == 0) return BLACK;
		//
		RayHit payload{};
		const auto& objects = this->_scene->objects;
		for (const auto& shape : objects) shape->hit(ray, payload);
		//
		if (payload) {
			// Scatter + compose
			SurfaceInfo surface{};
			Ray scattered = payload.obj->mat->scatter(ray, payload, surface);
			Color out = this->trace(scattered, depth - 1);
			out *= surface.attenuation;
			return out;
		}
		else {
			// Sky (athmosphere)
			static Color SkyColorA = Color(1.0f, 1.0f, 1.0f);
			static Color SkyColorB = Color(0.5f, 0.7f, 1.0f);
			f32 t = ray.dir.y / 2.0f + 0.5f;
			return Color::lerp(SkyColorA, SkyColorB, t);
		}
	}

} // namespace rt
