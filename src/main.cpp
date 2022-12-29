#include "MVRE/graphics/graphics_instance.hpp"
#include "MVRE/executioner/executioner.hpp"
#include "MVRE/engine/engine_handler.hpp"
#include <MVRE/graphics/backend/opengl/gl_backend_instance.hpp>
#include <MVRE/graphics/backend/vulkan/v_backend_instance.hpp>
#include <MVRE/math/vector4.hpp>
#include "scenes/test_scene.hpp"
#include <MVRE/scenes/scene_manager.hpp>
#include <MVRE/time/time_helper.hpp>
#include <MVRE/engine/layers/main_layers.hpp>
#include <MVRE/graphics/pipeline_manager.hpp>
#include <MVRE/engine/tick.hpp>
#include <MVRE/input/input_manager.hpp>

using namespace mvre_graphics;
using namespace mvre_math;
using namespace mvre_executioner;
using namespace mvre_engine;
using namespace mvre_scenes;
using namespace mvre_layers;

int main() {
    executioner::init();

    engine_handler engine = engine_handler();
    engine.init();

    engine.add_layer<load_layer>(load_layer_callback);
    engine.add_layer<update_layer>(update_layer_callback);
    engine.add_layer<render_layer>(render_layer_callback);

    auto g_instance = new gl_backend_instance(true);
    //auto g_instance = new v_backend_instance(true);
    auto instance = graphics_instance(g_instance);

    instance.create_with_window("MVRE", vector2(1920, 1080));

    auto new_scene = test_scene(&instance, &engine);

    scene_manager::add_scene("test", &new_scene);

    scene_manager::load_scene("test");

    engine.process_layer<load_layer>();

    tick update_tick(120);
    tick input_tick(240);

    while (instance.is_running()) {
        time_helper::perpare();
        instance.prepare_render();

        if (input_tick.tick_ready()) {
            instance.window_update();
            input_tick.reset();
        }

        if (update_tick.tick_ready()) {
            instance.update();
            engine.process_layer<update_layer>();
            instance.finish_update();
            update_tick.reset();
        }

        ///render
        engine.process_layer<render_layer>();
        executioner::execute();
        instance.draw();
        time_helper::update();
    }

    executioner::stop();
    mvre_resources::resource_manager::clean();
    engine.clean();
    pipeline_manager::destroy();
    executioner::clean();
    mvre_input::input_manager::clean();
    instance.destroy();
    return 0;
}