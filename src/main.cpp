#include <iostream>

#include <core/peng_engine.h>

int main()
{
    std::cout << "PengEngine starting..." << std::endl;

    PengEngine engine;
    engine.get_on_engine_initialized().subscribe([] {
        std::cout << "PengEngine started!" << std::endl;
    });

    engine.set_target_fps(60);
    engine.start();
}