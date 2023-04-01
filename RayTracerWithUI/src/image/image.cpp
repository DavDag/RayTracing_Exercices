#include "image.hpp"

#include "tga.hpp"

namespace rt {

	Image::Image(i32 w, i32 h):
		_w(w), _h(h), _data(nullptr)
	{
		_data = new u8[3 * w * h];
		memset(_data, (u8)0, sizeof(u8) * 3 * w * h);
	}

	void Image::set(i32 px, i32 py, Color pixel) {
		i32 index = 3 * ((_w * py) + px);
		_data[index + 0] = (u32(pixel.r * 255.0f)) & 0xff;
		_data[index + 1] = (u32(pixel.g * 255.0f)) & 0xff;
		_data[index + 2] = (u32(pixel.b * 255.0f)) & 0xff;
	}

	Color Image::get(i32 px, i32 py) const {
		i32 index = 3 * ((_w * py) + px);
		f32 r = (_data[index + 0] / 255.0f);
		f32 g = (_data[index + 1] / 255.0f);
		f32 b = (_data[index + 2] / 255.0f);
		return Color(r, g, b);
	}

	const u8* Image::rgb() const {
		return _data;
	}

	i32 Image::width() const {
		return _w;
	}

	i32 Image::height() const {
		return _h;
	}

	void Image::save(const std::string& filename) const {
		writeTGA(filename, _w, _h, _data);
	}

	void Image::open(const std::string& filename) const {
		std::ostringstream cmd;
		cmd << "start paintdotnet:\"" << filename << "\"";
		system(cmd.str().c_str());
	}

} // namespace rt
