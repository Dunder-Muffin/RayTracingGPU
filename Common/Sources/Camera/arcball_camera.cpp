#include "arcball_camera.h"
#include "Event/input.h"
#include "Time/time.h"
void ArcballCamera::calculate_transform()
{
  float y = rotation.y;
  float x = rotation.x ;
  vec3 direction = vec3(cos(x) * cos(y), sin(y), sin(x) * cos(y));
  
  transform.set_position(target_position - distance * direction);
  transform.set_rotation(PI*0.5-rotation.x, -rotation.y, 0);
}

void ArcballCamera::set_target(vec3 target)
{
  target_position = target;
}
void ArcballCamera::set_target(Transform* transform)
{
  target_transform = transform;
}
void ArcballCamera::mouse_move_handler(const MouseMoveEvent &e)
{
  if (rotationEnable)
  {
    float const pixToRad = PI / 180.f * 0.2f;
    rotation += vec2(e.dx, e.dy) * pixToRad;
  }
}
void ArcballCamera::mouse_click_handler(const MouseClickEvent &e)
{
  if (e.buttonType == MouseButtonType::MiddleButton)
  {
    rotationEnable = e.action == MouseButtonAction::Down;
  }
}
void ArcballCamera::mouse_wheel_handler(const MouseWheelEvent &)
{
  const float arcballCameraZoomSpeed = 0.07f;
  targetZoom -= Input::input().get_wheel() * arcballCameraZoomSpeed;
  targetZoom = glm::clamp(targetZoom, 0.05f, 1.0f);
}

void ArcballCamera::update()
{
  if (target_transform)
  {
    target_position = target_transform->get_position();
  }

  zoom = lerp(zoom, targetZoom, Time::delta_time() * 10);
  distance = maxdistance * zoom;
  calculate_transform();
}