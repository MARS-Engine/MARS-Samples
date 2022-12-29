#ifndef MVRE_SAMPLE_CAMERA_CONTROLLER_
#define MVRE_SAMPLE_CAMERA_CONTROLLER_

#include <MVRE/engine/component.hpp>
#include <MVRE/engine/layers/main_layers.hpp>

class camera_controller : public mvre_engine::component<camera_controller>, public mvre_layers::update_layer {
    void update() override;
};

#endif