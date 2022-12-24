#include <iostream>

#include <core/peng_engine.h>
#include <utils/io.h>

#include "blob_entity.h"

using namespace math;

namespace demo
{
    std::vector<Vector3f> vertices =
    {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f },
    };

    std::vector<Vector3f> colors =
    {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 },
    };

    std::vector<Vector3u> indices =
    {
        { 0, 1, 2 }
    };

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
        using namespace rendering;

        std::cout << "PengEngine starting..." << std::endl;

        PengEngine::get().on_engine_initialized().subscribe([&] {
            std::cout << "PengEngine started!" << std::endl;

            const std::string vertShaderSrc = io::read_text_file("shaders/demo/blob_v.glsl");
            const std::string fragShaderSrc = io::read_text_file("shaders/demo/blob_f.glsl");

            const auto shader = peng::make_shared<Shader>(vertShaderSrc, fragShaderSrc);
            const auto mesh = peng::make_shared<Mesh>(vertices, indices, colors);

            PengEngine::get().entity_manager().create_entity<BlobEntity>(shader, mesh, 100, 100, 10);
        });

        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();

        return 0;
    }
}