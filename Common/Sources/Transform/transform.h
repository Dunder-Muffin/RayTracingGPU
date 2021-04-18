#pragma once
#include "common.h"
#include "math.h"
#include "Shader/shader.h"
#include "component.h"
class Component;
class Transform : public Component
{
private:
  vec3 position;
  mat4x4 rotation;
  vec3 scale;
public:
  Transform();
  Transform(vec3 position, mat4x4 rotation = mat4x4(1.f), vec3 scale = vec3(1.f));
  Transform(vec3 position, vec3 rotation, vec3 scale = vec3(1.f));

  vec3& get_position();
  const vec3& get_position()const ;
  mat4x4 get_transform() const;
  mat3 get_rotation() const;
  vec3 get_forward() const;
  vec3 get_right() const;
  vec3 get_up() const;
  void set_rotation(const mat4& rotation);
  void set_rotation(float yaw = 0, float pitch = 0, float roll = 0);
  void set_position(const vec3 position);
  void set_scale(const vec3 scale);
  void set_to_shader(const Shader& shader) const;
};