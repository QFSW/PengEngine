#include <iostream>

#include <core/peng_engine.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class FadeEntity : public Entity
{
public:
    FadeEntity(float from[4], float to[4])
        : _time(0)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            _from[i] = from[i];
            _to[i] = to[i];
        }
    }

    virtual void tick(double delta_time) override
    {
        _time += delta_time;

        const float ratio = 0.5f + static_cast<float>(std::cos(_time / 500)) / -2.0f;
        const float ratio_inv = 1 - ratio;

        GLclampf color[4];
        for (uint8_t i = 0; i < 4; i++)
        {
            const float raw = ratio * _to[i] + ratio_inv * _from[i];
            color[i] = static_cast<GLclampf>(raw);
        }

        glClearColor(color[0], color[1], color[2], color[3]);
    }

    double get_age() const
    {
        return _time;
    }

private:
    float _from[4];
    float _to[4];
    double _time;
};

int main()
{
    std::cout << "PengEngine starting..." << std::endl;

    float gray[4] = { 0.5, 0.5, 0.5, 1.0 };
    float green[4] = { 0.0, 1, 0.0, 1.0 };
    float red[4] = { 1.0, 0.0, 0.0, 1.0 };

    std::weak_ptr<FadeEntity> green_entity;
    std::weak_ptr<FadeEntity> red_entity;

    PengEngine engine;
    engine.get_on_engine_initialized().subscribe([&] {
        std::cout << "PengEngine started!" << std::endl;
    });

    engine.get_on_frame_start().subscribe([&] {
        EntityManager& entity_manager = engine.get_entity_manager();

        if (green_entity.expired() && red_entity.expired())
        {
            green_entity = entity_manager.create_entity<FadeEntity>(gray, green);
        }

        if (!green_entity.expired() && green_entity.lock()->get_age() >= 5000)
        {
            entity_manager.destroy_entity(green_entity);
            red_entity = entity_manager.create_entity<FadeEntity>(gray, red);
        }

        if (!red_entity.expired() && red_entity.lock()->get_age() >= 5000)
        {
            entity_manager.destroy_entity(red_entity);
            green_entity = entity_manager.create_entity<FadeEntity>(gray, green);
        }
    });

    engine.set_target_fps(60);
    engine.start();
}