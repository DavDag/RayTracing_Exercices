#include "./src/image/image.hpp"
#include "./src/viewer/viewer.hpp"
#include "./src/raytracer/raytracer.hpp"
#include "./src/materials/diffusive.hpp"
#include "./src/materials/metallic.hpp"
#include "./src/materials/dielectric.hpp"
#include "./src/shapes/sphere.hpp"

#include <cstdlib>
#include <thread>
#include <iostream>

#ifdef _PROFILE
#include "./deps/profilerlib/profilerlib.hpp"
#endif // _PROFILE

std::shared_ptr<rt::Scene> rndScene();

int main(int argc, char* argv[]) {
    // Checks arguments
    if (argc != 4) {
        std::cout << "Usage: <scenefile> <configfile> <outputfile>\n";
        exit(EXIT_FAILURE);
    }

    // Load Scene & Options from file
    //std::shared_ptr<rt::Scene> scene = rndScene();
    std::shared_ptr<rt::Scene> scene = rt::Scene::FromFile(argv[1]);
    std::shared_ptr<rt::Options> options = rt::Options::FromFile(argv[2]);
    if (!scene || !options)
        exit(EXIT_FAILURE);
    std::cout << (*options) << "\n";
    std::cout << (*scene) << "\n";

    // Create Image, RayTracer & Viewer
    rt::Image image(scene->camera->imgW(), scene->camera->imgH());
    rt::RayTracer raytracer(scene, options, image);
    rt::Viewer viewer(image, 32, 4.0f);

    // Build RayTracer internal structs
    raytracer.build();

    // Launch Viewer in another thread
    std::thread tviewer([&viewer]() {
        viewer.init();
        viewer.start();
        viewer.exit();
    });

    // RayTracer
#ifdef _PROFILE
    profiler::Enable();
    profiler::FrameStart();
#endif // _PROFILE
    raytracer.process();
#ifdef _PROFILE
    profiler::FrameEnd();
    profiler::Disable();
    profiler::LogStats(profiler::GetStatsTable());
#endif // _PROFILE
    
    // Save to file
    image.save(argv[3]);

    // Wait Viewer
    tviewer.join();

    // Open result
#ifndef _PROFILE
    image.open(argv[3]);
#endif // _PROFILE
    exit(EXIT_SUCCESS);
}

std::shared_ptr<rt::Scene> rndScene() {
    //
    rt::SceneData data{};
    // Camera
    data.camera = std::make_shared<rt::Camera>(
        rt::Vec3(0.0f, 0.8f, 4.0f),
        rt::Vec3(0.0f, 0.0f, 0.0f),
        1920, 1080,
        45.0f
    );
    // Materials
    std::vector<std::shared_ptr<rt::Material>> materials{};
    int diffCount = 70, metalCount = 30, dieleCount = 10;
    for (int i = 0; i < diffCount; ++i) {
        std::string name = "diff_" + std::to_string(i);
        std::shared_ptr<rt::Diffusive> mat = std::make_shared<rt::Diffusive>(
            name,
            rt::Color::rnd()
        );
        materials.push_back(mat);
        data.materials.insert({ rt::crc32(name), mat });
    }
    for (int i = 0; i < metalCount; ++i) {
        std::string name = "metal_" + std::to_string(i);
        std::shared_ptr<rt::Metallic> mat = std::make_shared<rt::Metallic>(
            name,
            rt::Color::rnd(),
            rt::rnd_uniform<float>(0.0f, 1.0f)
        );
        materials.push_back(mat);
        data.materials.insert({ rt::crc32(name), mat });
    }
    for (int i = 0; i < dieleCount; ++i) {
        std::string name = "diele_" + std::to_string(i);
        std::shared_ptr<rt::Dielectric> mat = std::make_shared<rt::Dielectric>(
            name,
            rt::rnd_normal<float>(0.5f, 10.0f)
        );
        materials.push_back(mat);
        data.materials.insert({ rt::crc32(name), mat });
    }
    // Objects
    data.objects.push_back({
        std::make_shared<rt::Sphere>(
            "ground",
            materials[0],
            rt::Vec3(0.0f, -1000.0f, 0.0f),
            1000.0f
        )
        });
    for (int z = -15; z < 5; ++z) {
        for (float x = -5; x < 5; x += 0.5f) {
            if (rt::rnd_uniform<float>(0.0f, 1.0f) < 0.25f) continue;
            float offx = rt::rnd_uniform<float>(-0.25f, 0.25f);
            float offz = rt::rnd_uniform<float>(-0.25f, 0.25f);
            std::string name = "sphere_" + std::to_string(z) + "_" + std::to_string(x);
            data.objects.push_back({
                std::make_shared<rt::Sphere>(
                    name,
                    materials[rt::rnd_uniform<int>(0, (int)materials.size() - 1)],
                    rt::Vec3(x + offx, 0.099f, (float)z + offz),
                    0.1f
                )
                });
        }
    }
    //
    return std::make_shared<rt::Scene>(data);
}
