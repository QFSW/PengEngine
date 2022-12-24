#include <iostream>

#include <core/peng_engine.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "blob_entity.h"

namespace demo
{
    class FPSEntity : public Entity
    {
    public:
        FPSEntity()
            : Entity(true)
        { }

        virtual void tick(double delta_time) override
        {
            printf("Frametime = %.02fms\n", delta_time * 1000);
        }
    };

    int demo_main()
    {
        std::cout << "PengEngine starting..." << std::endl;

        PengEngine::get().on_engine_initialized().subscribe([&] {
            std::cout << "PengEngine started!" << std::endl;

            PengEngine::get().entity_manager().create_entity<BlobEntity>(100, 100, 10);
        });

        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();

        return 0;
    }
}