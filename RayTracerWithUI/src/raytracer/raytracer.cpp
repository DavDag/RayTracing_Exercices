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
			i32 samples = _options->samples;
			//for (i32 si = 0; si < samples; ++si) {
#ifdef _PROFILE
				// Single threaded
				for (i32 y = 0; y < h; ++y) {
					profiler::FrameStart();
					for (i32 x = 0; x < w; ++x)
							_process(x, y, si + 1);
					profiler::FrameEnd();
				}
#else
				// Multi threaded
				concurrency::parallel_for(0, h - 1, [&](i32 y) {
					for (i32 x = 0; x < w; ++x)
						_process(x, y, 1);
				});
#endif // _PROFILE
				//_image.updatePercentage((si + 1) / (f32)samples);
			//}
		}
		auto tend = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbeg).count();
		std::cout << "Time: " << delta << " (ms) or " << delta / 1000.0 << " (sec)\n";
	}

	void RayTracer::_process(i32 px, i32 py, i32 sampleIndex) {
		Color p = _pixel(px, py);
		//Color f = _image.add(px, py, p);
		//f /= (f32)sampleIndex;
		//f = f.gammaCorrected(2.0f);
		_image.set(px, py, p.gammaCorrected(2.0f));
	}

	Color RayTracer::_pixel(i32 px, i32 py) {
		Camera& camera = *_scene->camera;
		i32 w = camera.imgW(), h = camera.imgH();
		i32 maxdepth = _options->maxdepth;
		i32 samples = _options->samples;
		Color out = BLACK;
		for (i32 si = 0; si < samples; ++si) {
			f32 dx = (px + rnd_uniform<f32>(0.0f, 1.0f)) / (w - 1);
			f32 dy = (py + rnd_uniform<f32>(0.0f, 1.0f)) / (h - 1);
			Ray ray = camera.getRay(dx, dy);
			out += _trace(ray, maxdepth);
		}
		out /= (f32)samples;
		return out;
	}
	
	Color RayTracer::_trace(Ray& ray, i32 depth) {
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
				Color out = _trace(scattered, depth - 1);
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
