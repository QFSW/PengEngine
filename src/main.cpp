#include <iostream>

#include <core/peng_engine.h>

int main()
{
    std::cout << "PengEngine starting..." << std::endl;

    PengEngine engine;
    engine.set_target_fps(60);
    engine.start();
}