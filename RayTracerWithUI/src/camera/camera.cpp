#include "camera.hpp"

namespace rt {

	Camera::Camera(
		Vec3 pos, Vec3 target,
		i32 imgw, i32 imgh,
		f32 fovy, f32 aperture
	):
		_pos(pos), _target(target), 
		_imgw(imgw), _imgh(imgh),
		_fovy(fovy), _aperture(aperture)
	{
		_aspect = imgw / (f32)imgh;

		// Viewport
		f32 theta = degToRad(fovy);
		f32 viewh = 2.0f * std::tan(theta / 2.0f);
		f32 vieww = _aspect * viewh;

		// Lookat
		_w = Vec3::unit(_pos - _target);
		_u = Vec3::cross(Vec3(0, 1, 0), _w);
		_v = Vec3::cross(_w, _u);

		// Increments per-axis
		f32 focusDist = (pos - target).length();
		_xInc = _u * vieww * focusDist;
		_yInc = _v * viewh * focusDist;
		_lowLeft = _pos - _xInc / 2.0f - _yInc / 2.0f - _w * focusDist;
	}

	Ray Camera::getRay(f32 dx, f32 dy) const {
		// Project using increment pex-axis
		Vec3 xinc = _xInc * dx;
		Vec3 yinc = _yInc * (1.0f - dy);
		Vec3 diskPos = Vec3::rndInUnitDisk() * (this->_aperture / 2.0f);
		Vec3 off = _u * diskPos.x + _v * diskPos.y;

		// Create ray
		Vec3 rayOrig = _pos + off;
		Vec3 rayDir = _lowLeft + xinc + yinc - _pos - off;

		return Ray(rayOrig, Vec3::unit(rayDir));
	}

	void Camera::print(std::ostream& out) const {
		out << "pos = " << _pos << "\n"
			<< "target = " << _target << "\n"
			<< "view = " << _imgw << " " << _imgh << "\n"
			<< "fovy = " << _fovy << "\n"
			<< "aperture = " << _aperture << "\n"
			<< "#_aspect = " << _aspect << "\n"
			<< "#_xInc = " << _xInc << "\n"
			<< "#_yInc = " << _yInc << "\n";
	}

	i32 Camera::imgW() const {
		return _imgw;
	}

	i32 Camera::imgH() const {
		return _imgh;
	}

} // namespace rt
