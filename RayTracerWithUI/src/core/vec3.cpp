#include "vec3.hpp"

#include "core.hpp"

namespace rt {

	Vec3::Vec3(): Vec3(0.0f) { }
	Vec3::Vec3(f32 v): Vec3(v, v, v) { }
	Vec3::Vec3(f32 x, f32 y, f32 z):
		x(x), y(y), z(z)
	{
		DBG_ASSERT(hasNaNs() == false);
	}

	bool Vec3::hasNaNs() const {
		return std::isnan(this->x) || std::isnan(this->y) || std::isnan(this->z);
	}

	std::ostream& operator<<(std::ostream& out, const Vec3& vec) {
		out << vec.x << " " << vec.y << " " << vec.z;
		return out;
	}

	Vec3& Vec3::operator+=(const Vec3& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Vec3& Vec3::operator-=(const Vec3& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Vec3& Vec3::operator*=(f32 scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Vec3& Vec3::operator/=(f32 scalar) {
		f32 tmp = 1.0f / scalar;
		this->x *= tmp;
		this->y *= tmp;
		this->z *= tmp;
		DBG_ASSERT(hasNaNs() == false);
		return (*this);
	}

	Vec3 Vec3::operator-() const {
		f32 nx = -this->x;
		f32 ny = -this->y;
		f32 nz = -this->z;
		return Vec3(nx, ny, nz);
	}

	Vec3 Vec3::operator+(const Vec3& other) const {
		f32 nx = this->x + other.x;
		f32 ny = this->y + other.y;
		f32 nz = this->z + other.z;
		return Vec3(nx, ny, nz);
	}

	Vec3 Vec3::operator-(const Vec3& other) const {
		f32 nx = this->x - other.x;
		f32 ny = this->y - other.y;
		f32 nz = this->z - other.z;
		return Vec3(nx, ny, nz);
	}

	Vec3 Vec3::operator*(f32 scalar) const {
		f32 nx = this->x * scalar;
		f32 ny = this->y * scalar;
		f32 nz = this->z * scalar;
		return Vec3(nx, ny, nz);
	}

	Vec3 Vec3::operator/(f32 scalar) const {
		f32 tmp = 1.0f / scalar;
		f32 nx = this->x * tmp;
		f32 ny = this->y * tmp;
		f32 nz = this->z * tmp;
		return Vec3(nx, ny, nz);
	}

	f32 Vec3::length() const {
		return std::sqrtf(this->lengthSquared());
	}

	f32 Vec3::lengthSquared() const {
		f32 x2 = this->x * this->x;
		f32 y2 = this->y * this->y;
		f32 z2 = this->z * this->z;
		return x2 + y2 + z2;
	}

	bool Vec3::isZero() const {
		constexpr f32 pos = +0.00001f;
		constexpr f32 neg = -0.00001f;
		return (this->x < pos && this->x > neg)
			&& (this->y < pos && this->y > neg)
			&& (this->z < pos && this->z > neg);
	}

	Vec3 Vec3::unit(const Vec3& vec) {
		f32 lenght = vec.length();
		return vec / lenght;
	}

	f32 Vec3::dot(const Vec3& veca, const Vec3& vecb) {
		f32 x = veca.x * vecb.x;
		f32 y = veca.y * vecb.y;
		f32 z = veca.z * vecb.z;
		return x + y + z;
	}

	Vec3 Vec3::cross(const Vec3& veca, const Vec3& vecb) {
		f32 x = veca.y * vecb.z - vecb.y * veca.z;
		f32 y = veca.z * vecb.x - vecb.z * veca.x;
		f32 z = veca.x * vecb.y - vecb.x * veca.y;
		return Vec3(x, y, z);
	}

	Vec3 Vec3::reflect(const Vec3& vec, const Vec3& surfNorm) {
		return vec - surfNorm * (Vec3::dot(vec, surfNorm) * 2.0f);
	}

	Vec3 Vec3::lerp(const Vec3& veca, const Vec3& vecb, f32 t) {
		return veca * (1.0f - t) + vecb * t;
	}

	Vec3 Vec3::rnd() {
		f32 x = rnd_uniform<f32>(-1.0f, 1.0f);
		f32 y = rnd_uniform<f32>(-1.0f, 1.0f);
		f32 z = rnd_uniform<f32>(-1.0f, 1.0f);
		return Vec3(x, y, z);
	}

	Vec3 Vec3::rndInUnitSphere() {
		f32 x = rnd_normal<f32>(0.0f, 1.0f);
		f32 y = rnd_normal<f32>(0.0f, 1.0f);
		f32 z = rnd_normal<f32>(0.0f, 1.0f);
		return Vec3::unit(Vec3(x, y, z));
	}

} // namespace rt
