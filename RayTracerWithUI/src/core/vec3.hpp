#pragma once

#include "types.hpp"

#include <ostream>

namespace rt {

	class Vec3 {
	public:
		explicit Vec3();
		explicit Vec3(f32 v);
		explicit Vec3(f32 x, f32 y, f32 z);

		bool hasNaNs() const;

		friend std::ostream& operator<<(std::ostream& out, const Vec3& vec);

		Vec3& operator+=(const Vec3& other);
		Vec3& operator-=(const Vec3& other);
		Vec3& operator*=(f32 scalar);
		Vec3& operator/=(f32 scalar);

		Vec3 operator-() const;

		Vec3 operator+(const Vec3& other) const;
		Vec3 operator-(const Vec3& other) const;
		Vec3 operator*(f32 scalar) const;
		Vec3 operator/(f32 scalar) const;

		f32 length() const;
		f32 lengthSquared() const;

		bool isZero() const;

		static Vec3 unit(const Vec3& vec);
		static f32 dot(const Vec3& veca, const Vec3& vecb);
		static Vec3 cross(const Vec3& veca, const Vec3& vecb);
		static Vec3 reflect(const Vec3& vec, const Vec3& surfNorm);
		static Vec3 lerp(const Vec3& veca, const Vec3& vecb, f32 t);

		static Vec3 rnd();
		static Vec3 rndInUnitSphere();

	public:
		f32 x, y, z;
	};

} // namespace rt
