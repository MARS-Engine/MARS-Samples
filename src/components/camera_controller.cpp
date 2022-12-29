#include "camera_controller.hpp"
#include <MVRE/time/time_helper.hpp>

void camera_controller::update() {
    float speed = 40;
    float look_speed = 5;
    mvre_math::vector3<float> movement = { 0, 0, 0 };

    if (get_input()->get_key("A"))
        movement.x(-speed);
    if (get_input()->get_key("D"))
        movement.x(speed);
    if (get_input()->get_key("S"))
        movement.z(-speed);
    if (get_input()->get_key("W"))
        movement.z(speed);

    transform()->set_position(transform()->position() + movement * time_helper::delta_time());
    transform()->set_rotation(transform()->rotation() * mvre_math::quaternion<float>::from_axis_angle(mvre_math::vector3<float>(get_input()->mouse_relative().y(), get_input()->mouse_relative().x(), 0.0f) * look_speed, time_helper::delta_time()));
}