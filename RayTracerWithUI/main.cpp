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
    if (argc < 4) {
        std::cout << "Usage: <scenefile> <configfile> <outputfile> [--preview]\n";
        exit(EXIT_FAILURE);
    }
    bool preview = (argc > 4 && strcmp(argv[4], "--preview") == 0);

    // Load Scene & Options from file
    //std::shared_ptr<rt::Scene> scene = rndScene();
    std::shared_ptr<rt::Scene> scene = rt::Scene::FromFile(argv[1]);
    std::shared_ptr<rt::Options> options = rt::Options::FromFile(argv[2]);
    if (!scene || !options)
        exit(EXIT_FAILURE);
    std::cout << (*options) << "\n";
    std::cout << (*scene) << "\n";

    // Create Image & RayTracer
    rt::Image image(scene->camera->imgW(), scene->camera->imgH());
    rt::RayTracer raytracer(scene, options, image);

    // Build RayTracer internal structures
    raytracer.build();

    // Launch Viewer in another thread
    rt::Viewer viewer(preview, image, 32, 4.0f);
    std::thread tviewer([&viewer]() {
        viewer.init();
        viewer.start();
        viewer.exit();
    });

    // RayTracer
#ifdef _PROFILE
    profiler::Enable();
    profiler::FrameStart();
    raytracer.process();
    profiler::FrameEnd();
    profiler::Disable();
    profiler::LogStats(profiler::GetStatsTable());
#else
    raytracer.process();
#endif // _PROFILE
    
    // Save to file
    image.save(argv[3]);

    // Wait Viewer
    tviewer.join();

    // Open result
    if (preview) image.open(argv[3]);
    exit(EXIT_SUCCESS);
}

std::shared_ptr<rt::Scene> rndScene() {
    rt::SceneData data{};
    data.camera = std::make_shared<rt::Camera>(
        rt::Vec3(3.0f, 0.6f, 3.0f),
        rt::Vec3(0.0f, 0.0f, 0.0f),
        1080, 720,
        30.0f, 0.01f, 4.0f
    );
    data.materials.insert({
        rt::crc32("groundMat"),
        std::make_shared<rt::Diffusive>("groundMat", rt::Color(0.5, 0.5, 0.5))
    });
    data.materials.insert({
        rt::crc32("diffMat"),
        std::make_shared<rt::Diffusive>("diffMat", rt::Color::rnd())
    });
    data.materials.insert({
        rt::crc32("metalMat"),
        std::make_shared<rt::Metallic>("metalMat", rt::Color::rnd(), 0.01f)
    });
    data.materials.insert({
        rt::crc32("dielMat"),
        std::make_shared<rt::Dielectric>("dielMat", 1.5f)
    });
    data.objects.push_back(
        std::make_shared<rt::Sphere>(
            "ground",
            data.materials.at(rt::crc32("groundMat")),
            rt::Vec3(0.0f, -100.0f, 0.0f),
            100.0f
        )
    );
    data.objects.push_back(
        std::make_shared<rt::Sphere>(
            "midSphere",
            data.materials.at(rt::crc32("dielMat")),
            rt::Vec3(0.0f, 0.5f, 0.0f),
            0.5f
        )
    );
    data.objects.push_back(
        std::make_shared<rt::Sphere>(
            "leftSphere",
            data.materials.at(rt::crc32("diffMat")),
            rt::Vec3(-1.0f, 0.5f, 0.0f),
            0.5f
        )
    );
    data.objects.push_back(
        std::make_shared<rt::Sphere>(
            "rightSphere",
            data.materials.at(rt::crc32("metalMat")),
            rt::Vec3(+1.0f, 0.5f, 0.0f),
            0.5f
        )
    );
    for (float z = -3; z < 3; z += 0.5f) {
        for (float x = -5; x < 5; x += 0.25f) {
            if (z >= -0.25f && z <= 0.25f) continue;
            if (x >= -0.25f && x <= 0.25f) continue;
            if (rt::rnd_uniform<float>(0.0f, 1.0f) < 0.5f) continue;
            //
            float offx = rt::rnd_uniform<float>(-1.0f, 1.0f) * 0.05f;
            float offz = rt::rnd_uniform<float>(-1.0f, 1.0f) * 0.05f;
            float objrad = 0.075f;
            rt::Vec3 objpos = rt::Vec3(x + offx, objrad, (float)z + offz);
            //
            std::string objname = "sphere_" + std::to_string(z) + "_" + std::to_string(x);
            std::string matname = "mat_" + objname;
            //
            float matchoice = rt::rnd_uniform<float>(0.0f, 1.0f);
            std::shared_ptr<rt::Material> objmat(nullptr);
            if (matchoice < 0.7f) {
                rt::Color matcol = rt::Color::rnd();
                objmat = std::make_shared<rt::Diffusive>(matname, matcol);
            }
            else if (matchoice < 0.95f) {
                rt::Color matcol = rt::Color::rnd();
                float matfuzziness = rt::rnd_uniform<float>(0.0001f, 1.0f);
                objmat = std::make_shared<rt::Metallic>(matname, matcol, matfuzziness);
            }
            else {
                float refractionRatio = rt::rnd_uniform<float>(1.0f, 20.0f);
                objmat = std::make_shared<rt::Dielectric>(matname, refractionRatio);
                data.objects.push_back(std::make_shared<rt::Sphere>(objname, objmat, objpos, objrad*0.95f));
            }
            data.materials.insert({ rt::crc32(matname), objmat });
            data.objects.push_back(std::make_shared<rt::Sphere>(objname, objmat, objpos, objrad));
        }
    }
    //
    return std::make_shared<rt::Scene>(data);
}
