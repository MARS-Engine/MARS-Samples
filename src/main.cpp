#include "MARS/graphics/graphics_instance.hpp"
#include "MARS/executioner/executioner.hpp"
#include "MARS/engine/engine_handler.hpp"
#include <MARS/graphics/backend/vulkan/v_backend_instance.hpp>
#include <MARS/math/vector4.hpp>
#include "scenes/test_scene.hpp"
#include <MARS/scenes/scene_manager.hpp>
#include <MARS/engine/layers/main_layers.hpp>
#include <MARS/graphics/pipeline_manager.hpp>
#include <MARS/engine/tick.hpp>
#include <MARS/input/input_manager.hpp>
#include <MPE/engine_layer.hpp>

using namespace mars_graphics;
using namespace mars_math;
using namespace mars_executioner;
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
    executioner::init();

    engine_handler engine = engine_handler();
    engine.init();

    engine.add_layer<load_layer>(load_layer_callback);
    engine.add_layer<update_layer>(update_layer_callback);
    engine.add_layer<post_update_layer>(post_update_layer_callback);
    engine.add_layer<post_render_layer>(post_render_layer_callback);
    engine.add_layer<render_layer>(render_layer_callback);
    engine.add_layer<mpe::mpe_layer>(mpe::mpe_update_layer_callback);

    auto g_instance = new v_backend_instance(true);

    auto instance = graphics_instance(g_instance);

    instance.create_with_window("MARS", vector2<size_t>(1920, 1080), "deferred.mr");

    auto new_scene = test_scene(&instance, &engine);

    scene_manager::add_scene("test", &new_scene);

    scene_manager::load_scene("test");

    engine.spawn_wait_list();

    engine.process_layer<load_layer>();

    //update tick rate must be way higher than refresh rate, or it feels like it feels like its lagging
    tick update_tick(std::numeric_limits<float>::max());
    tick input_tick(240);
    tick fps_tick(2);

    time_calc update_time;
    time_calc render_time;

    bool waiting_render_finish = false;

    while (instance.is_running()) {
        if (input_tick.tick_ready()) {
            instance.window_update();
            input_tick.reset();
        }

        if (update_tick.tick_ready()) {
            update_time.start();
            instance.update();
            engine.lock();
            engine.process_layer<mpe::mpe_layer>();
            engine.process_layer<update_layer>();
            engine.process_layer<post_update_layer>();
            engine.unlock();
            instance.finish_update();
            update_tick.reset();
            update_time.end();
        }

        if (executioner::finished()) {
            if (waiting_render_finish) {
                instance.draw();
                render_time.start();
            }

            waiting_render_finish = true;
            instance.prepare_render();
            engine.lock();
            engine.process_layer<render_layer>();
            engine.process_layer<post_render_layer>();
            engine.unlock();
            executioner::execute();
            render_time.end();
        }

        //if (fps_tick.tick_ready()) {
        //    update_time.print("update - ");
        //    render_time.print("render - ");
        //    fps_tick.reset();
        //}

        //For some reason putting the render layer in its own thread to be constantly running is 1/3 slower, more research needed
    }

    executioner::stop();

    while (!executioner::finished()) { }

    instance.wait_idle();

    mars_resources::resource_manager::clean();
    engine.clean();
    pipeline_manager::destroy();
    executioner::clean();
    mars_input::input_manager::clean();
    instance.destroy();

    return 0;
}