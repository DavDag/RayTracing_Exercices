#include "image.hpp"

namespace rt {

	Image::Image(i32 w, i32 h):
		_w(w), _h(h), _data(nullptr)
	{
		this->_data = new u8[3 * w * h];
		memset(this->_data, (u8)0, sizeof(u8) * 3 * w * h);
	}

	void Image::set(i32 px, i32 py, Pixel pixel) {
		i32 index = 3 * ((this->_w * py) + px);
		this->_data[index + 0] = (u32(pixel.r * 255.0f)) & 0xff;
		this->_data[index + 1] = (u32(pixel.g * 255.0f)) & 0xff;
		this->_data[index + 2] = (u32(pixel.b * 255.0f)) & 0xff;
	}

	Pixel Image::get(i32 px, i32 py) const {
		i32 index = 3 * ((this->_w * py) + px);
		f32 r = (this->_data[index + 0] / 255.0f);
		f32 g = (this->_data[index + 1] / 255.0f);
		f32 b = (this->_data[index + 2] / 255.0f);
		return Pixel(r, g, b);
	}

	const u8* Image::rgb() const {
		return this->_data;
	}

	i32 Image::width() const {
		return this->_w;
	}

	i32 Image::height() const {
		return this->_h;
	}

} // namespace rt
