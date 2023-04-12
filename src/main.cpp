#include "MARS/graphics/graphics_engine.hpp"
#include "MARS/engine/object_engine.hpp"
#include <MARS/graphics/backend/vulkan/vulkan_backend.hpp>
#include <MARS/math/vector4.hpp>
#include "scenes/test_scene.hpp"
#include <MARS/scenes/scene_manager.hpp>
#include <MARS/engine/layers/main_layers.hpp>
#include <MARS/engine/engine_worker.hpp>
#include <MARS/graphics/pipeline_manager.hpp>
#include <MARS/engine/tick.hpp>
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
        printf("%s%f\n", _prefix.c_str(), std::chrono::duration<float, std::chrono::seconds::period>(m_end - m_start).count());
    }
};

int main() {
    auto resources = std::make_shared<mars_resources::resource_manager>();

    auto engine = std::make_shared<object_engine>(2);
    engine->set_resources(mars_ref<mars_resources::resource_manager>(resources));

    auto update_worker = engine->create_worker(std::thread::hardware_concurrency() / 2);
    auto render_worker = engine->create_worker(std::thread::hardware_concurrency() / 2);

    engine->add_layer<load_layer>(load_layer_callback, true);
    engine->add_layer<update_layer>(update_layer_callback);
    engine->add_layer<post_update_layer>(post_update_layer_callback);
    engine->add_layer<post_render_layer>(post_render_layer_callback);
    engine->add_layer<update_gpu>(update_gpu_callback);
    engine->add_layer<mpe::mpe_layer>(mpe::mpe_update_layer_callback);

    auto v_graphics = vulkan_backend(true);
    v_graphics.set_resources(mars_ref<mars_resources::resource_manager>(resources));

    auto graphics = std::make_shared<graphics_engine>(&v_graphics, 1);
    v_graphics.set_graphics(mars_ref<graphics_engine>(graphics));
    graphics->create_with_window("MARS", vector2<size_t>(1920, 1080), "deferred.mr");

    auto new_scene = test_scene(mars_ref<graphics_engine>(graphics), mars_ref<object_engine>(engine));

    scene_manager::add_scene("test", &new_scene);

    scene_manager::load_scene("test");

    engine->spawn_wait_list();

    //update tick rate must be way higher than refresh rate, or it feels like it feels like its lagging
    tick_rate update_tick(480);
    tick_rate input_tick(240);

    time_calc update_time;
    time_calc render_time;

    auto thread = std::thread([&]{
        while (graphics->is_running()) {
            render_time.start();
            graphics->prepare_render();
            graphics->draw();
            render_worker->process_layer<update_gpu>().wait();
            graphics->wait_draw();
            render_worker->process_layer<post_render_layer>().wait();
            graphics->swap();
            render_time.end();
            engine->spawn_wait_room();
        }
    });

    while (graphics->is_running()) {
        if (input_tick.tick_ready()) {
            graphics->window_update();
            input_tick.reset();
        }

        if (update_tick.tick_ready()) {
            update_time.start();
            update_worker->process_layer<load_layer>().wait();
            graphics->update();
            update_worker->process_layer<mpe::mpe_layer>().wait();
            update_worker->process_layer<update_layer>().wait();
            update_worker->process_layer<post_update_layer>().wait();
            graphics->finish_update();
            update_tick.reset();
            update_time.end();
            engine->spawn_wait_room();
        }
    }

    thread.join();

    update_worker->close();
    render_worker->close();
    update_worker->join();
    render_worker->join();

    resources->clean();
    engine->clean();
    pipeline_manager::destroy();
    mars_input::input_manager::clean();
    graphics->destroy();

    return 0;
}