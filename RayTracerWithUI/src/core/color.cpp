#include "color.hpp"

#include "core.hpp"

namespace rt {

	Color::Color(): Color(0) { }
	Color::Color(f32 v): Color(v, v, v) { }
	Color::Color(f32 r, f32 g, f32 b)
		: r(r), g(g), b(b)
	{
		DBG_ASSERT(hasNaNs() == false);
	}

	bool Color::hasNaNs() const {
		return std::isnan(this->r) || std::isnan(this->g) || std::isnan(this->b);
	}

	std::ostream& operator<<(std::ostream& out, const Color& color) {
		out << color.r << "," << color.g << "," << color.b;
		return out;
	}

	Color Color::gammaCorrected(f32 gamma) const {
		f32 exp = 1.0f / gamma;
		f32 nr = std::powf(this->r, exp);
		f32 ng = std::powf(this->g, exp);
		f32 nb = std::powf(this->b, exp);
		return Color(nr, ng, nb);
	}

	Color& Color::operator+=(const Color& other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Color& Color::operator*=(const Color& other) {
		this->r *= other.r;
		this->g *= other.g;
		this->b *= other.b;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Color& Color::operator/=(f32 scalar) {
		f32 tmp = 1.0f / scalar;
		this->r *= tmp;
		this->g *= tmp;
		this->b *= tmp;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Color Color::lerp(const Color& a, const Color& b, f32 t) {
		f32 it = 1.0f - t;
		f32 nr = a.r * it + b.r * t;
		f32 ng = a.g * it + b.g * t;
		f32 nb = a.b * it + b.b * t;
		return Color(nr, ng, nb);
	}

} // namespace rt
