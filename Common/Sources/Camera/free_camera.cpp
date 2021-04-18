/*#include "free_camera.h"
#include "Event/input.h"
#include "Time/time.h"

void FreeCamera::calculate_transform()
{
  transform.set_rotation(-rotation.x, -rotation.y, 0);
}
void FreeCamera::mouse_move_handler(const MouseMoveEvent &e)
{
  if (rotationEnable)
  {
    float const pixToRad = PI / 180.f * 0.2f;
    rotation += vec2(e.dx, e.dy) * pixToRad;

    calculate_transform();
  }
}
void FreeCamera::space_button_handler(const KeyboardEvent &)
{
  rotationEnable = !rotationEnable;
}

void FreeCamera::update()
{
  
  vec3 delta = transform.get_forward() * (Input::input().get_key(SDLK_w) - Input::input().get_key(SDLK_s)) +
              transform.get_right() * (Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a)) +
              transform.get_up() * (Input::input().get_key(SDLK_e) - Input::input().get_key(SDLK_c));
  
  const float minSpeed = 50.6f;
  const float maxSpeed = 200.6f;
  float speed = lerp(minSpeed, maxSpeed, Input::input().get_key(SDLK_LSHIFT));
  transform.get_position() += delta * speed * Time::delta_time();
}
*/