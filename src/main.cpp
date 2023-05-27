#include "MARS/graphics/graphics_engine.hpp"
#include "MARS/engine/object_engine.hpp"
#include <MARS/graphics/backend/vulkan/vulkan_backend.hpp>
#include <MARS/math/vector4.hpp>
#include "scenes/test_scene.hpp"
#include <MARS/scenes/scene_manager.hpp>
#include <MARS/engine/layers/main_layers.hpp>
#include <MARS/engine/engine_worker.hpp>
#include <MARS/input/input_manager.hpp>
#include <MPE/engine_layer.hpp>

using namespace mars_graphics;
using namespace mars_math;
using namespace mars_engine;
using namespace mars_scenes;
using namespace mars_layers;

class time_calc {
private:
    std::chrono::_V2::system_clock::time_point m_start;
    std::chrono::_V2::system_clock::time_point m_end;
public:
    void start() {
        m_start = std::chrono::high_resolution_clock::now();
    }

    void end() {
        m_end = std::chrono::high_resolution_clock::now();
    }

    void print(const std::string& _prefix) {
        printf("%s%f\n", _prefix.c_str(), std::chrono::duration<float, std::chrono::milliseconds::period>(m_end - m_start).count());
    }
};

int main() {
    //debug float point errors
    //feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);

    auto engine = std::make_shared<object_engine>(2);

    auto update_worker = engine->create_worker(std::thread::hardware_concurrency() / 2);
    auto render_worker = engine->create_worker(std::thread::hardware_concurrency() / 2);

    engine->add_layer<load_layer>(load_layer_callback, true);
    engine->add_layer<update_layer>(update_layer_callback);
    engine->add_layer<post_update_layer>(post_update_layer_callback);
    engine->add_layer<post_render_layer>(post_render_layer_callback);
    engine->add_layer<update_gpu>(update_gpu_callback);
    engine->add_layer<mpe::mpe_layer>(mpe::mpe_update_layer_callback);

    auto v_graphics = std::make_shared<vulkan_backend>(true);
    v_graphics->set_resources(mars_ref<mars_resources::resource_manager>(engine->get<mars_resources::resource_manager>()));

    auto graphics = std::make_shared<graphics_engine>(v_graphics, 1);
    graphics->set_engine(engine);
    v_graphics->set_graphics(mars_ref<graphics_engine>(graphics));
    graphics->create_with_window("MARS", vector2<int>(1920, 1080), "deferred.mr");

    engine->set_graphics(mars_ref<graphics_engine>(graphics));

    auto new_scene = test_scene(mars_ref<graphics_engine>(graphics), mars_ref<object_engine>(engine));

    auto s_manager = engine->get<scene_manager>();

    s_manager->add_scene("test", &new_scene);

    s_manager->load_scene("test");

    engine->spawn_wait_list();

    time_calc update_time;
    time_calc render_time;

    render_worker->
        add_function([&]() {
            render_time.start();
            graphics->prepare_render();
            graphics->draw();
        })
        .add_layer<update_gpu>()
        .add_function([&]() {
            graphics->wait_draw();
        })
        .add_layer<post_render_layer>()
        .add_function([&]() {
            graphics->swap();
            render_time.end();
            engine->spawn_wait_room();
        });

    update_worker->add_function([&]() {
            graphics->window_update();
            update_time.start();
        })
        .add_layer<load_layer>()
        .add_function([&]() {
            graphics->update();
        })
        .add_layer<mpe::mpe_layer>()
        .add_layer<update_layer>()
        .add_layer<post_update_layer>()
        .add_function([&]() {
            graphics->finish_update();
            engine->spawn_wait_room();
            update_time.end();
            printf("%f\n", engine->get_layer(typeid(update_layer))->m_tick.delta_ms());
        });

    update_worker->run();
    render_worker->run();

    graphics->is_running()->wait(false);
    graphics->wait_draw();

    update_worker->close();
    render_worker->close();

    engine->get<mars_resources::resource_manager>()->clean();

    engine->clean();
    graphics->destroy();

    return 0;
}