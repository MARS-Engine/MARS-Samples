#include "test_scene.hpp"
#include <MVRE/engine/component.hpp>
#include <MVRE/engine/engine_handler.hpp>
#include <MVRE_3D/mesh_renderer.hpp>
#include <MVRE/components/camera_updater.hpp>

using namespace mvre_engine;
using namespace mvre_3d;
void test_scene::load() {
    auto obj = new engine_object();

    obj->transform()->set_position({ -2, 0, 2});

    auto renderer = obj->add_component<mesh_renderer>();

    renderer->set_mesh_path("engine/assets/mesh/monkey.obj");
    renderer->set_material("engine/assets/materials/mesh.mat");

    obj->add_component<mvre_component::camera_updater>();

    m_engine->instance(obj, m_instance, nullptr);
}