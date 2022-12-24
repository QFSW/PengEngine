#include <iostream>

#include <core/peng_engine.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "blob_entity.h"

namespace demo
{
    std::vector<float> vertices =
    {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    std::vector<GLuint> indices =
    {
        0, 1, 3,
        1, 2, 3
    };

    std::string vertShaderSrc =
        "\n#version 330 core"
        "\nlayout(location = 0) in vec3 aPos;"
        "\n"
        "\nout vec4 vertexColor;"
        "\n"
        "\nvoid main()"
        "\n{"
        "\n	gl_Position = vec4(aPos, 1.0);"
        "\n vertexColor = vec4(0.5, 0.0, 0.0, 1.0);"
        "\n}\0";

    std::string fragShaderSrc =
        "\n#version 330 core"
        "\nout vec4 FragColor;"
        "\nin vec4 vertexColor;"
        "\n"
        "\nvoid main()"
        "\n{"
        "\n	FragColor = vertexColor;"
        "\n}\0";

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

            auto shader = peng::make_shared<Shader>(vertShaderSrc, fragShaderSrc);
            auto mesh = peng::make_shared<Mesh>(vertices, indices);

            PengEngine::get().entity_manager().create_entity<BlobEntity>(shader, mesh, 100, 100, 10);
        });

        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();

        return 0;
    }
}