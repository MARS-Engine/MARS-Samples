#include "camera_controller.hpp"

mars_math::vector2<float> angle = { 0, 0 };

void camera_controller::update(const mars_engine::tick& _tick) {
    float speed = 5;
    float look_speed = 30;
    mars_math::vector3<float> movement = { 0, 0, 0 };

    auto input = engine()->get_singleton<mars_input::input>();

    if (input->get_key("A"))
        movement.x = -speed;
    if (input->get_key("D"))
        movement.x = speed;
    if (input->get_key("S"))
        movement.z = -speed;
    if (input->get_key("W"))
        movement.z = speed;

    angle.x = angle.x + input->mouse_relative().y * look_speed * _tick.delta();
    angle.y = angle.y + input->mouse_relative().x * look_speed * _tick.delta();

    if (angle.x > 89)
        angle.x = 89;
    else if (angle.x < -89)
        angle.x = -89;

    transform().set_position(transform().position() + transform().rotation() * (movement * _tick.delta()));
    transform().set_rotation(
            mars_math::quaternion<float>::from_axis_angle(mars_math::vector3<float>::up(), angle.y * mars_math::DEG2RAD) *
            mars_math::quaternion<float>::from_axis_angle(mars_math::vector3<float>::right(), angle.x * mars_math::DEG2RAD)
            );
}