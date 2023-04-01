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
		return std::isnan(r) || std::isnan(g) || std::isnan(b);
	}

	std::ostream& operator<<(std::ostream& out, const Color& color) {
		out << color.r << " " << color.g << " " << color.b;
		return out;
	}

	Color Color::gammaCorrected(f32 gamma) const {
		f32 exp = 1.0f / gamma;
		f32 nr = std::powf(r, exp);
		f32 ng = std::powf(g, exp);
		f32 nb = std::powf(b, exp);
		return Color(nr, ng, nb);
	}

	Color& Color::operator+=(const Color& other) {
		r += other.r;
		g += other.g;
		b += other.b;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Color& Color::operator*=(const Color& other) {
		r *= other.r;
		g *= other.g;
		b *= other.b;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Color& Color::operator/=(f32 scalar) {
		f32 tmp = 1.0f / scalar;
		r *= tmp;
		g *= tmp;
		b *= tmp;
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

	Color Color::rnd() {
		f32 r = rnd_uniform<f32>(0.0f, 1.0f);
		f32 g = rnd_uniform<f32>(0.0f, 1.0f);
		f32 b = rnd_uniform<f32>(0.0f, 1.0f);
		return Color(r, g, b);
	}

} // namespace rt
