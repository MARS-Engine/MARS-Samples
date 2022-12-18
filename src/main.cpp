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

    while (instance.is_running()) {
        time_helper::perpare();
        ///Update
        instance.update();
        engine.process_layer<update_layer>();

        ///render
        instance.prepare_render();
        engine.process_layer<render_layer>();
        executioner::execute();
        instance.draw();
        time_helper::update();
    }

    executioner::stop();
    mvre_resources::resource_manager::clean();
    engine.clean();
    executioner::clean();
    instance.destroy();
    return 0;
}