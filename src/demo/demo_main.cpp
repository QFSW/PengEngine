#include <iostream>

#include <core/peng_engine.h>

#include "blob_entity.h"

using namespace math;
using namespace rendering;

namespace demo
{
    std::vector<Vector3f> vertices =
    {
        { -1, -1, 0 },
        { 1, -1, 0},
        { -1, 1, 0 },
        { 1, 1, 0 }
    };

    std::vector<Vector3u> indices =
    {
        { 0, 1, 2 },
        { 2, 1, 3 }
    };

    std::vector<Vector2f> tex_coords =
    {
        { 0, 0 },
        { 1, 0 },
        { 0, 1 },
        { 1, 1 }
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
        std::cout << "PengEngine starting..." << std::endl;

        PengEngine::get().on_engine_initialized().subscribe([&] {
            std::cout << "PengEngine started!" << std::endl;

            const auto shader = peng::make_shared<Shader>("shaders/demo/blob_v.glsl", "shaders/demo/blob_f.glsl");
            const auto material = peng::make_shared<Material>(shader);
            const auto mesh = peng::make_shared<Mesh>(vertices, indices, std::vector<Vector3f>(), tex_coords);

            const Vector2i blob_grid(15, 12);

            for (int32_t blob_x = 0; blob_x < blob_grid.x ; blob_x++)
            {
                for (int32_t blob_y = 0; blob_y < blob_grid.y; blob_y++)
                {
                    const auto material_copy = peng::copy_shared(material);
                    const Vector2f pos = Vector2f(blob_x - (blob_grid.x - 1) / 2.0f, blob_y - (blob_grid.y - 1) / 2.0f) * 30;

                    PengEngine::get().entity_manager().create_entity<BlobEntity>(mesh, material_copy, pos, 20.0f);
                }
            }
        });

        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();

        return 0;
    }
}