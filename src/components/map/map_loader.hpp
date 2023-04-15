#ifndef MARS_MAP_LOADER
#define MARS_MAP_LOADER

#include <MARS/engine/component.hpp>
#include <MARS/engine/layers/main_layers.hpp>
#include <MARS/loaders/wavefront_loader.hpp>
#include <MARS/resources/ram_texture.hpp>

class map_loader : public mars_engine::component, public mars_layers::update_layer, public mars_layers::post_update_layer {
private:
    std::string m_map;
    mars_ref<mars_resources::ram_texture> m_heightmap;
    std::shared_ptr<mars_loader::mesh<mars_loader::wave_vertex>> ptr;
    mars_loader::mesh<mars_loader::wave_vertex> _mesh;
public:
    void set_map(const std::string& _map) {
        m_map = _map;
    }

    void update(const mars_engine::tick& _tick) override;
    void post_update() override;
};

#endif