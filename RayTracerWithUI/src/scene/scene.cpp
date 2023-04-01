#include "scene.hpp"

#include "../materials/diffusive.h"
#include "../shapes/sphere.hpp"

namespace rt {

	Scene::Scene(const SceneData& data):
		camera(data.camera),
		materials(data.materials),
		objects(data.objects)
	{

	}

	void ReadCamera(std::ifstream& in, SceneData& data) {
		Vec3 pos(0.0f);
		Vec3 target(0.0f);
		f32 fovy = 0.0f;
		bool bPos = false;
		bool bTarget = false;
		bool bFovy = false;
		//
		std::string line{}, prop{}, equal{};
		while (std::getline(in, line)) {
			if (line.size() == 0) continue;
			if (line[0] == '#') continue;
			std::istringstream ss(line);
			ss >> prop >> equal;
			switch (crc32(prop)) {
			case crc32("pos"):
				bPos = true;
				ss >> pos.x >> pos.y >> pos.z;
				break;
			case crc32("target"):
				bTarget = true;
				ss >> target.x >> target.y >> target.z;
				break;
			case crc32("fovy"):
				bFovy = true;
				ss >> fovy;
				break;
			default:
				break;	
			}
			//
			if (bPos && bTarget && bFovy) break;
		}
		//
		data.camera = std::make_shared<Camera>(pos, target, fovy);
	}

	void ReadMatDiffusive(std::ifstream& in, std::string& name, SceneData& data) {
		Vec3 albedo(0.0f);
		bool bAlbedo = false;
		//
		std::string line{}, prop{}, equal{};
		while (std::getline(in, line)) {
			if (line.size() == 0) continue;
			if (line[0] == '#') continue;
			std::istringstream ss(line);
			ss >> prop >> equal;
			switch (crc32(prop)) {
			case crc32("albedo"):
				bAlbedo = true;
				ss >> albedo.x >> albedo.y >> albedo.z;
				break;
			default:
				break;
			}
			//
			if (bAlbedo) break;
		}
		//
		data.materials.insert({
			crc32(name),
			std::make_shared<Diffusive>(name)
		});
	}

	void ReadObjSphere(std::ifstream& in, std::string& name, SceneData& data) {
		std::string matName{};
		std::shared_ptr<Material> mat(nullptr);
		bool bMat = false;
		//
		std::string line{}, prop{}, equal{};
		while (std::getline(in, line)) {
			if (line.size() == 0) continue;
			if (line[0] == '#') continue;
			std::istringstream ss(line);
			ss >> prop >> equal;
			switch (crc32(prop)) {
			case crc32("mat"):
				bMat = true;
				ss >> matName;
				mat = data.materials.at(crc32(matName));
				break;
			default:
				break;
			}
			//
			if (bMat) break;
		}
		//
		data.objects.push_back(std::make_shared<Sphere>(name, mat));
	}

	std::unique_ptr<Scene> Scene::FromFile(const std::string& filename) {
		std::ifstream in(filename);
		if (!in.is_open()) {
			std::cerr << "Unable to open scene file:" << filename << "\n";
			return std::unique_ptr<Scene>(nullptr);
		}
		//
		SceneData data {
			.camera = std::unique_ptr<Camera>(nullptr),
			.materials = std::unordered_map<u32, std::shared_ptr<Material>>(),
			.objects = std::vector<std::shared_ptr<Shape>>()
		};
		//
		std::string line{}, tag{}, name{};
		while (std::getline(in, line)) {
			if (line.size() == 0) continue;
			if (line[0] == '#') continue;
			std::istringstream ss(line);
			ss >> tag;
			switch (crc32(tag)) {
				case crc32("CAMERA"):
					ReadCamera(in, data);
					break;
				case crc32("MAT_DIFF"):
					ss >> name;
					ReadMatDiffusive(in, name, data);
					break;
				case crc32("OBJ_SPHERE"):
					ss >> name;
					ReadObjSphere(in, name, data);
					break;
				default:
					std::cerr << "Unrecognized tag: " << tag << "\n";
					break;
			}
		}
		//
		return std::make_unique<Scene>(data);
	}

} // namespace rt
