#ifndef MARS_MAP_LOADER
#define MARS_MAP_LOADER

#include <MARS/engine/component.hpp>
#include <MARS/engine/layers/main_layers.hpp>
#include <MARS/loaders/wavefront_loader.hpp>
#include <MARS/resources/ram_texture.hpp>

class map_loader : public mars_engine::component, mars_layers::load_layer {
private:
    std::string m_map;
    mars_ref<mars_resources::ram_texture> m_heightmap;
    std::shared_ptr<mars_loader::mesh<mars_loader::wave_vertex>> m_mesh;
public:
    void set_map(const std::string& _map) {
        m_map = _map;
    }

    void load() override;
};

#endif