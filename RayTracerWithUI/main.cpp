#include "./src/core/core.hpp"
#include "./src/scene/scene.hpp"
#include "./src/options/options.hpp"
#include "./src/image/image.hpp"
#include "./src/viewer/viewer.hpp"

#include <cstdlib>
#include <thread>
#include <iostream>

int main(int argc, char* argv[]) {
    // Checks arguments
    if (argc != 3) {
        std::cout << "Usage: <scenefile> <configfile>\n";
        exit(EXIT_FAILURE);
    }

    // Load scene & options from file
    std::unique_ptr<rt::Scene> scene = rt::Scene::FromFile(argv[1]);
    std::unique_ptr<rt::Options> options = rt::Options::FromFile(argv[2]);
    if (!scene || !options)
        exit(EXIT_FAILURE);

    // Create image & viewer
    rt::Image image(options->w, options->h);
    rt::Viewer viewer(image, 32, 4.0f);

    // Launch viewer in another thread
    std::thread tviewer([&viewer]() {
        viewer.init();
        viewer.start();
        viewer.exit();
    });

    // Start RayTracer
    
    // Join & exit
    tviewer.join();
    exit(EXIT_SUCCESS);
}
