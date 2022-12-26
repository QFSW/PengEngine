#include <core/peng_engine.h>
#include <rendering/texture.h>
#include <rendering/primitives.h>

#include "blob_entity.h"

using namespace math;
using namespace rendering;

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

    class ShutdownEntity : public Entity
    {
    public:
        ShutdownEntity(double max_age)
            : Entity(true)
			, _age(0)
			, _max_age(max_age)
        { }

        virtual void tick(double delta_time) override
        {
            _age += delta_time;
            if (_age >= _max_age)
            {
                PengEngine::get().request_shutdown();
            }
        }

    private:
        double _age;
        double _max_age;
    };

    int demo_main()
    {
	    const int32_t handle = PengEngine::get().on_engine_initialized().subscribe([&]
	    {
		    const peng::shared_ref<const Shader> shader = peng::make_shared<Shader>(
			    "resources/shaders/demo/blob.vert",
			    "resources/shaders/demo/blob.frag"
		    );

		    const peng::shared_ref<const Texture> texture = peng::make_shared<Texture>("wall",
			    "resources/textures/demo/wall.jpg"
		    );

		    auto material = peng::make_shared<Material>(shader);
		    material->set_parameter("color_tex", texture);

		    const Vector2i blob_grid(5, 4);
		    for (int32_t blob_x = 0; blob_x < blob_grid.x; blob_x++)
		    {
			    for (int32_t blob_y = 0; blob_y < blob_grid.y; blob_y++)
			    {
				    const Vector2f pos = Vector2f(blob_x - (blob_grid.x - 1) / 2.0f, blob_y - (blob_grid.y - 1) / 2.0f)
					    * 300;
				    PengEngine::get().entity_manager().create_entity<BlobEntity>(Primitives::cube(), material, pos, 175.0f);
			    }
		    }
	    }, "demo_start");

        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();
        PengEngine::get().on_engine_initialized().unsubscribe(handle);

        return 0;
    }
}