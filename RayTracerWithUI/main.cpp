#include "./src/image/image.hpp"
#include "./src/viewer/viewer.hpp"
#include "./src/raytracer/raytracer.hpp"

#include <cstdlib>
#include <thread>
#include <iostream>

int main(int argc, char* argv[]) {
    // Checks arguments
    if (argc != 4) {
        std::cout << "Usage: <scenefile> <configfile> <outputfile>\n";
        exit(EXIT_FAILURE);
    }

    // Load Scene & Options from file
    std::shared_ptr<rt::Scene> scene = rt::Scene::FromFile(argv[1]);
    std::shared_ptr<rt::Options> options = rt::Options::FromFile(argv[2]);
    if (!scene || !options)
        exit(EXIT_FAILURE);

    // Create Image, RayTracer & Viewer
    rt::Image image(options->w, options->h);
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

    // Start RayTracer
    raytracer.process();
    image.save(argv[3]);
    
    // Join & exit
    tviewer.join();
    image.open(argv[3]);
    exit(EXIT_SUCCESS);
}
