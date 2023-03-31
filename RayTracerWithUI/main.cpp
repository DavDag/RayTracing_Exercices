#include "./src/core/core.hpp"
#include "./src/scene/scene.hpp"
#include "./src/options/options.hpp"
#include "./src/image/image.hpp"
#include "./src/viewer/viewer.hpp"

#include <cstdlib>
#include <thread>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: <scenefile> <configfile>\n";
        exit(EXIT_FAILURE);
    }
    //
    std::unique_ptr<rt::Scene> scene = rt::Scene::FromFile(argv[1]);
    std::unique_ptr<rt::Options> options = rt::Options::FromFile(argv[2]);
    if (!scene || !options)
        exit(EXIT_FAILURE);
    //
    rt::Image image(scene->w, scene->h);
    rt::Viewer viewer(image, 32, 4.0f);
    std::thread tviewer([&viewer]() {
        viewer.init();
        viewer.start();
        viewer.exit();
    });
    //
    tviewer.join();
    exit(EXIT_SUCCESS);
}
