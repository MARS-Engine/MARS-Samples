#include "map_loader.hpp"

std::atomic<size_t> m_testing = 0;

void map_loader::update(const mars_engine::tick& _tick) {
    m_testing++;
}

void map_loader::post_update() {
    m_testing = 0;
}