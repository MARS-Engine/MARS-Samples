#include "test_scene.hpp"
#include <MARS/engine/component.hpp>
#include <MARS_3D/mesh_renderer.hpp>
#include <MARS/components/camera_updater.hpp>
#include <MARS/graphics/light_manager.hpp>
#include <MPE/rigid_body.hpp>
#include <MPE/colliders/AABB.hpp>
#include "../components/camera/camera_controller.hpp"

using namespace mars_engine;
using namespace mars_3d;
void test_scene::load() {

    //Lights

    float constant  = 1.0;
    float linear    = 0.7;
    float quadratic = 1.8;
    float lightMax  = std::fmaxf(std::fmaxf(1.0f, 0.0f), 0.0f);

    auto n_light = new mars_graphics::point_light({
        .Position = { 2, 1, 2 },
        .Color = { 1.0f, 0.0f, 0.0f },
        .Linear = 0.7f,
        .Quadratic = 1.8f,
        .Radius = (-linear +  (float)std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax))) / (2 * quadratic)
    });

    auto n_light_2 = new mars_graphics::point_light({
        .Position = { 2, 1, 1 },
        .Color = { 0.0f, 1.0f, 0.0f },
        .Linear = 0.7f,
        .Quadratic = 1.8f,
        .Radius = (-linear +  (float)std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax))) / (2 * quadratic)
    });

    m_graphics->backend()->lights()->add_light(n_light);
    m_graphics->backend()->lights()->add_light(n_light_2);

    //others
    mars_object obj = create_object(m_engine);
    obj->add_component<mars_component::camera_updater>();
    obj->add_component<camera_controller>();

    m_engine->spawn(obj, m_graphics);

    obj = create_object(m_engine);

    obj->transform().set_position({ -2, 20, 2});
    obj->add_component<mpe::rigid_body>();

    auto renderer = obj->add_component<mesh_renderer>();

    renderer->set_mesh_path("engine/assets/mesh/monkey.obj");
    renderer->set_material("engine/assets/materials/mesh.mat");

    mars_ref<mars_loader::wavefront_mesh> mesh;

    m_engine->resources()->load_resource("engine/assets/mesh/monkey.obj", mesh);
    obj->add_component<mpe::AABB>()->load_from_mesh(mesh);

    m_engine->spawn(obj, m_graphics);

    auto ground = create_object(m_engine);

    ground->transform().set_position({ 0, -2, 0});
    ground->transform().set_scale({ 10, 0.5, 10});

    renderer = ground->add_component<mesh_renderer>();

    renderer->set_mesh_path("engine/assets/mesh/cube.obj");
    renderer->set_material("engine/assets/materials/uv_mesh.mat");

    m_engine->resources()->load_resource("engine/assets/mesh/cube.obj", mesh);
    ground->add_component<mpe::AABB>()->load_from_mesh(mesh);

    m_engine->spawn(ground, m_graphics);

    //auto cube = new engine_object();
//
    //cube->transform().set_position({ 2, 2, 2});
//
    //renderer = cube->add_component<mesh_renderer>();
    //cube->add_component<mpe::rigid_body>();
//
    //renderer->set_mesh_path("engine/assets/mesh/cube.obj");
    //renderer->set_material("engine/assets/materials/uv_mesh.mat");
//
    //mars_resources::resource_manager::load_resource("engine/assets/mesh/cube.obj", mesh);
    //cube->add_component<mpe::AABB>()->load_from_mesh(mesh);
//
    //m_engine->instance(cube, m_instance, nullptr);
//
    //cube = new engine_object();
//
    //cube->transform().set_position({ 2.5, 5, 2});
//
    //renderer = cube->add_component<mesh_renderer>();
    //cube->add_component<mpe::rigid_body>();
//
    //renderer->set_mesh_path("engine/assets/mesh/cube.obj");
    //renderer->set_material("engine/assets/materials/uv_mesh.mat");
//
    //mars_resources::resource_manager::load_resource("engine/assets/mesh/cube.obj", mesh);
    //cube->add_component<mpe::AABB>()->load_from_mesh(mesh);

    //m_engine->instance(cube, m_instance, nullptr);

    //for (auto x = 0; x < 5; x++) {
    //    for (auto y = 0; y < 5; y++) {
    //        for (auto z = 0; z < 5; z++) {
    //            ground = new engine_object();
//
    //            ground->transform().set_position({ (float)x, (float)y, (float)z});
    //            ground->transform().set_scale({ 0.5f, 0.5f, 0.5f});
//
    //            renderer = ground->add_component<mesh_renderer>();
//
    //            renderer->set_mesh_path("engine/assets/mesh/cube.obj");
    //            renderer->set_material("engine/assets/materials/uv_mesh.mat");
//
    //            m_engine->instance(ground, m_instance, nullptr);
    //        }
    //    }
    //}
}