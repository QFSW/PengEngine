#include <iostream>

#include <core/peng_engine.h>

int main()
{
    std::cout << "PengEngine starting..." << std::endl;

    PengEngine engine;
    engine.get_on_engine_initialized().subscribe([] {
        std::cout << "PengEngine started!" << std::endl;
    });

    int32_t handle = engine.get_on_frame_start().subscribe([&] {
        std::cout << "PengEngine frame 0!" << std::endl;
        engine.get_on_frame_start().unsubscribe(handle);
    });

    engine.set_target_fps(60);
    engine.start();
}