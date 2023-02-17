#ifndef MARS_SAMPLE_CAMERA_CONTROLLER_
#define MARS_SAMPLE_CAMERA_CONTROLLER_

#include <MARS/engine/component.hpp>
#include <MARS/engine/layers/main_layers.hpp>

class camera_controller : public mars_engine::component, public mars_layers::update_layer {
    void update() override;
};

#endif