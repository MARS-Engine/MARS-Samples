#include "test_scene.hpp"
#include <MVRE/engine/component.hpp>
#include <MVRE/engine/engine_handler.hpp>
#include <MVRE_3D/mesh_renderer.hpp>
#include <MVRE/components/camera_updater.hpp>

using namespace mvre_engine;

void test_scene::load() {
    auto obj = new engine_object();
    obj->add_component<mesh_renderer>();
    obj->add_component<mvre_component::camera_updater>();

    m_engine->instance(obj, m_instance, nullptr);
}