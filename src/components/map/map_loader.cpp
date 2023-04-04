#include "map_loader.hpp"
#include <MARS_3D/mesh_renderer.hpp>

void map_loader::load() {
    engine()->resources()->load_graphical_resource(m_map, m_heightmap, graphics());

    mars_loader::mesh<mars_loader::wave_vertex> _mesh;

    _mesh.vertices.reserve(m_heightmap->size().x() * m_heightmap->size().y());

    for (size_t x = 0; x < m_heightmap->size().x(); x++) {
        for (size_t y = 0; y < m_heightmap->size().y(); y++) {
            mars_loader::wave_vertex v;
            v.vertex = { (float)x, 0 ,(float)y };
            _mesh.vertices.push_back(v);
        }
    }

    auto renderer = object()->add_component<mars_3d::mesh_renderer>();
    renderer->set_material("engine/assets/materials/mesh.mat");
}