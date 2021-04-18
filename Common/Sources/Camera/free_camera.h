/*#pragma once
#include "camera.h"


class FreeCamera: public Camera, public IUpdatable
{
 private:
  vec2 rotation;
  bool rotationEnable = true;
  void calculate_transform();
public: 
  FreeCamera(vec3 position, vec2 rotation):
  rotation(rotation)
  { 
    transform.set_position(position);
    calculate_transform();
  }
  void mouse_move_handler(const MouseMoveEvent &e);
  void space_button_handler(const KeyboardEvent &e);
  virtual void update() override;

};
*/