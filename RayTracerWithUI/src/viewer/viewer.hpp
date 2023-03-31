#pragma once

#include "../image/image.hpp"

struct GLFWwindow;

namespace rt {

	class Viewer {
	public:
		Viewer(const Image& image, i32 padding, f32 updatePerSec);

		void init();
		void start();
		void stop();
		void exit();

	private:
		void preview();
		void update();

	private:
		bool _valid;
		i32 _ww, _wh, _pd;
		const Image& _image;
		GLFWwindow* _window;
		f32 _updatePerSec;
		f64 _accTimeSinceLastUpdateSec;
		u32 _imageTex;
	};

} // namespace rt
