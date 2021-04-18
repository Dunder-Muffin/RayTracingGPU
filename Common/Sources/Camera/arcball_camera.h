#pragma once
#include "camera.h"


class ArcballCamera: public Camera, public IUpdatable
{
 private:
  float maxdistance, zoom, targetZoom, distance;
  vec2 rotation;
  vec3 target_position;
  Transform *target_transform;
  void calculate_transform();
  bool rotationEnable = false;
public: 
  ArcballCamera(Transform *target, float distance, vec2 rotation):
  maxdistance(distance), zoom(0.2f), targetZoom(zoom), distance(zoom * maxdistance), rotation(rotation), target_position(vec3()), target_transform(target)
  { 
    calculate_transform();
  }
  ArcballCamera(vec3 target, float distance, vec2 rotation):
  maxdistance(distance), zoom(0.2f), targetZoom(zoom), distance(zoom * maxdistance), rotation(rotation), target_position(target), target_transform(nullptr)
  { 
    calculate_transform();
  }
  void set_target(vec3 target);
  void set_target(Transform* transform);

  void mouse_move_handler(const MouseMoveEvent &e);
  void mouse_click_handler(const MouseClickEvent &e);
  void mouse_wheel_handler(const MouseWheelEvent &e);
  virtual void update() override;
};