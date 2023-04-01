#include "raytracer.hpp"

#include "rayhit.hpp"
#include <ppl.h>

#ifdef _PROFILE
#include "../../deps/profilerlib/profilerlib.hpp"
#endif // _PROFILE

namespace rt {

	RayTracer::RayTracer(
		const std::shared_ptr<Scene>& scene,
		const std::shared_ptr<Options>& options,
		Image& image
	):
		_scene(scene), _options(options), _image(image)
	{
		//
	}

	void RayTracer::build() {
		//
	}

	void RayTracer::process() {
		auto tbeg = std::chrono::high_resolution_clock::now();
		{
			i32 w = _scene->camera->imgW();
			i32 h = _scene->camera->imgH();
#ifdef _PROFILE
			for (i32 y = 0; y < h; ++y) {
				profiler::FrameStart();
#else
			concurrency::parallel_for(0, h-1, [&](i32 y) {
#endif // _PROFILE
				for (i32 x = 0; x < w; ++x) {
					Color p = pixel(x, y);
#ifdef _DEBUG
					if ((p.r > 1.0f || p.g > 1.0f || p.b > 1.0f)
						|| (p.r < 0.0f || p.g < 0.0f || p.b < 0.0f))
					{
						__debugbreak();
						exit(1);
					}
#endif // _DEBUG
					p = p.gammaCorrected(2.0f);
					_image.set(x, y, p);
				}
#ifdef _PROFILE
				profiler::FrameEnd();
			}
#else
			});
#endif // _PROFILE
		}
		auto tend = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbeg).count();
		std::cout << "Time: " << delta << " (ms) or " << delta / 1000.0 << " (sec)\n";
	}

	Color RayTracer::pixel(i32 px, i32 py) {
		Camera& camera = *_scene->camera;
		i32 w = camera.imgW(), h = camera.imgH();
		i32 samples = _options->samples;
		i32 maxdepth = _options->maxdepth;
		Color out(0.0f);
		for (i32 si = 0; si < samples; ++si) {
			f32 dx = (px + rnd_uniform<f32>(0.0f, 1.0f)) / (w-1);
			f32 dy = (py + rnd_uniform<f32>(0.0f, 1.0f)) / (h-1);
			Ray ray = camera.getRay(dx, dy);
			out += trace(ray, maxdepth);
		}
		out /= (f32)samples;
		return out;
	}
	
	Color RayTracer::trace(Ray& ray, i32 depth) {
		if (depth == 0) return BLACK;
		//
		RayHit payload{};
		const auto& objects = _scene->objects;
		for (const auto& shape : objects) shape->hit(ray, payload);
		//
		if (payload) {
			// Scatter + compose
			SurfaceInfo surface{};
			Ray scattered{};
			if (payload.obj->mat->scatter(ray, payload, surface, scattered)) {
				Color out = trace(scattered, depth - 1);
				out *= surface.attenuation;
				return out;
			}
			return BLACK;
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
