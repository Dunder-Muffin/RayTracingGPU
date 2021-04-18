#pragma once
#include "common.h"
#include "math.h"
#include "Transform/transform.h"
#include "Event/input.h"
#include "Shader/shader.h"
#include "component.h"
class Camera : public Component
{
protected:
  Transform transform;
  mat4x4 projection;
  int priority = 0;
public:
  void set_orthographic(float width, float height, float zNear, float zFar);
  void set_perspective(float fieldOfView, float aspectRatio, float zNear, float zFar);
  void set_perspective(float fieldOfView, float zNear, float zFar);

  const mat4x4& get_projection() const;
  Transform& get_transform();
  mat4x4 get_transform_matrix() const;
  void set_priority(int priority);
  int get_priority() const;
  void set_to_shader(const Shader& shader, bool sky_box = false) const;
};

typedef shared_ptr<Camera> CameraPtr;
void add_camera(CameraPtr camera);
CameraPtr  main_camera(); 
