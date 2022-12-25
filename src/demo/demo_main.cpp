#include <iostream>

#include <core/peng_engine.h>
#include <rendering/texture.h>

#include "blob_entity.h"

using namespace math;
using namespace rendering;

namespace demo
{
    std::vector<Vector3f> vertices =
    {
        Vector3f(-1, -1, -1) * 0.5f,
	    Vector3f(1, -1, -1) * 0.5f,
	    Vector3f(1, 1, -1) * 0.5f,
	    Vector3f(-1, 1, -1) * 0.5f,
	    Vector3f(-1, -1, 1) * 0.5f,
	    Vector3f(1, -1, 1) * 0.5f,
	    Vector3f(1, 1, 1) * 0.5f,
	    Vector3f(-1, 1, 1) * 0.5f
    };

    std::vector<Vector3u> indices =
    {
        Vector3u(0, 1, 3),
    	Vector3u(3, 1, 2),
	    Vector3u(1, 5, 2),
    	Vector3u(2, 5, 6),
	    Vector3u(5, 4, 6),
    	Vector3u(6, 4, 7),
	    Vector3u(4, 0, 7),
    	Vector3u(7, 0, 3),
	    Vector3u(3, 2, 7),
    	Vector3u(7, 2, 6),
	    Vector3u(4, 5, 0),
    	Vector3u(0, 5, 1)
    };

    std::vector<Vector3f> colors =
    {
        Vector3f(1, 0, 0),
        Vector3f(0, 1, 0),
        Vector3f(0, 0, 1),
        Vector3f(1, 1, 0),
        Vector3f(0, 1, 1),
        Vector3f(1, 0, 1),
        Vector3f(1, 1, 1),
        Vector3f(0, 0, 0),
    };

    std::vector<Vector2f> tex_coords =
    {
        Vector2f(0, 0),
	    Vector2f(1, 0),
	    Vector2f(1, 1),
	    Vector2f(0, 1),

        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0)
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

            const auto shader = peng::make_shared<Shader>(
				"resources/shaders/demo/blob_v.glsl",
				"resources/shaders/demo/blob_f.glsl"
            );

            const auto texture = peng::make_shared<Texture> (
                "resources/textures/demo/wall.jpg"
            );

        	auto material = peng::make_shared<Material>(shader);
            material->set_parameter("color_tex", texture);

            const auto mesh = peng::make_shared<Mesh>(vertices, indices, colors, tex_coords);

            const Vector2i blob_grid(10, 8);

            for (int32_t blob_x = 0; blob_x < blob_grid.x ; blob_x++)
            {
                for (int32_t blob_y = 0; blob_y < blob_grid.y; blob_y++)
                {
                    const Vector2f pos = Vector2f(blob_x - (blob_grid.x - 1) / 2.0f, blob_y - (blob_grid.y - 1) / 2.0f) * 150;
                    PengEngine::get().entity_manager().create_entity<BlobEntity>(mesh, material, pos, 100.0f);
                }
            }
        });

        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();

        return 0;
    }
}