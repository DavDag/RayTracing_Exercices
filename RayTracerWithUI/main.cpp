#include <cstdlib>
#include <thread>

#include "./src/core/core.hpp"
#include "./src/viewer/viewer.hpp"

i32 main(i32 argc, u8* argv[]) {
    i32 w = 1920, h = 1080;
    //
    rt::Image image(w, h);
    rt::Viewer viewer(image, 32, 4.0f);
    std::thread tviewer([&viewer]() {
        viewer.init();
        viewer.start();
        viewer.exit();
    });
    //
    for (i32 i = 0; i < 1'000; ++i) {
        for (i32 j = 0; j < 100; ++j) {
            i32 px = i32(((f32)rand() / (RAND_MAX + 1)) * w);
            i32 py = i32(((f32)rand() / (RAND_MAX + 1)) * h);
            image.set(px, py, rt::Pixel(1.0f));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //
    tviewer.join();
    exit(EXIT_SUCCESS);
}
