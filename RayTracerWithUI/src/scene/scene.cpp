#include "scene.hpp"

#include "../materials/diffusive.hpp"
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
		i32 vieww = 0, viewh = 0;
		bool bPos = false;
		bool bTarget = false;
		bool bFovy = false;
		bool bView = false;
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
			case crc32("view"):
				bView = true;
				ss >> vieww >> viewh;
				break;
			default:
				break;	
			}
			//
			if (bPos && bTarget && bFovy && bView) break;
		}
		//
		data.camera = std::make_shared<Camera>(pos, target, vieww, viewh, fovy);
	}

	void ReadMatDiffusive(std::ifstream& in, std::string& name, SceneData& data) {
		Color albedo(0.0f);
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
				ss >> albedo.r >> albedo.g >> albedo.b;
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
			std::make_shared<Diffusive>(name, albedo)
		});
	}

	void ReadObjSphere(std::ifstream& in, std::string& name, SceneData& data) {
		std::string matName{};
		std::shared_ptr<Material> mat(nullptr);
		Vec3 cen(0.0f);
		f32 rad = 0.0f;
		bool bMat = false;
		bool bCen = false;
		bool bRad = false;
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
			case crc32("cen"):
				bCen = true;
				ss >> cen.x >> cen.y >> cen.z;
				break;
			case crc32("rad"):
				bRad = true;
				ss >> rad;
				break;
			default:
				break;
			}
			//
			if (bMat && bCen && bRad) break;
		}
		//
		data.objects.push_back(std::make_shared<Sphere>(name, mat, cen, rad));
	}

	std::shared_ptr<Scene> Scene::FromFile(const std::string& filename) {
		std::ifstream in(filename);
		if (!in.is_open()) {
			std::cerr << "Unable to open scene file:" << filename << "\n";
			return std::shared_ptr<Scene>(nullptr);
		}
		//
		SceneData data {
			.camera = std::shared_ptr<Camera>(nullptr),
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
#ifdef _DEBUG
		std::cout << "\n=======SCENE=======\n";
		std::cout << "[MainCamera]\n";
		data.camera->print(std::cout);
		std::cout << "\n";
		for (const auto& mat : data.materials) {
			mat.second->print(std::cout);
			std::cout << "\n";
		}
		for (const auto& obj : data.objects) {
			obj->print(std::cout);
			std::cout << "\n";
		}
#endif // _DEBUG
		return std::make_unique<Scene>(data);
	}

} // namespace rt
