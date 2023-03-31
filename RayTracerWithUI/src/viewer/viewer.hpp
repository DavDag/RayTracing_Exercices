#pragma once

namespace rt {

	class Image;
	
	class Viewer {
	public:
		Viewer(const Image& image, int padding, float updatePerSec);

		void init();
		void start();
		void stop();
		void exit();

	private:
		void preview();
		void update();

	private:
		bool _valid;
		int _ww, _wh, _pd;
		const Image& _image;
		void* _window;
		float _updatePerSec;
		double _accTimeSinceLastUpdateSec;
		unsigned int _imageTex;
	};

} // namespace rt
