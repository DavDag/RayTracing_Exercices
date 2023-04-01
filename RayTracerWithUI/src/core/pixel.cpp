#include "pixel.hpp"

namespace rt {

	Pixel::Pixel(): Pixel(0) { }
	Pixel::Pixel(f32 v): Pixel(v, v, v) { }
	Pixel::Pixel(f32 r, f32 g, f32 b): r(r), g(g), b(b) { }

	Pixel Pixel::gammaCorrected(f32 gamma) const {
		f32 exp = 1.0f / gamma;
		f32 nr = std::powf(this->r, exp);
		f32 ng = std::powf(this->g, exp);
		f32 nb = std::powf(this->b, exp);
		return Pixel(nr, ng, nb);
	}

	Pixel& Pixel::operator+=(const Pixel& other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		return (*this);
	}

	Pixel& Pixel::operator/=(f32 scalar) {
		f32 tmp = 1.0f / scalar;
		this->r *= tmp;
		this->g *= tmp;
		this->b *= tmp;
		return (*this);
	}

	std::ostream& Pixel::operator<<(std::ostream& out) const {
		out << this->r << "," << this->g << "," << this->b;
		return out;
	}

	Pixel Pixel::lerp(const Pixel& a, const Pixel& b, f32 t) {
		f32 it = 1.0f - t;
		f32 nr = a.r * it + b.r * t;
		f32 ng = a.g * it + b.g * t;
		f32 nb = a.b * it + b.b * t;
		return Pixel(nr, ng, nb);
	}

} // namespace rt
