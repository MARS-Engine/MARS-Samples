#include "test_scene.hpp"
#include <MVRE/engine/component.hpp>
#include <MVRE_3D/mesh_renderer.hpp>
#include <MVRE/components/camera_updater.hpp>
#include "../components/camera_controller.hpp"

using namespace mvre_engine;
using namespace mvre_3d;
void test_scene::load() {
    auto obj = new engine_object();
    obj->add_component<mvre_component::camera_updater>();
    obj->add_component<camera_controller>();

    m_engine->instance(obj, m_instance, nullptr);

    obj = new engine_object();

    obj->transform()->set_position({ -2, 0, 2});

    auto renderer = obj->add_component<mesh_renderer>();

    renderer->set_mesh_path("engine/assets/mesh/monkey.obj");
    renderer->set_material("engine/assets/materials/mesh.mat");

    m_engine->instance(obj, m_instance, nullptr);

    auto ground = new engine_object();

    ground->transform()->set_position({ 0, -2, 0});
    ground->transform()->set_scale({ 10, 0.5, 10});

    renderer = ground->add_component<mesh_renderer>();

    renderer->set_mesh_path("engine/assets/mesh/cube.obj");
    renderer->set_material("engine/assets/materials/uv_mesh.mat");

    m_engine->instance(ground, m_instance, nullptr);
}