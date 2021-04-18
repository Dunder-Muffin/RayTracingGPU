#include "direction_light.h"

DirectionLight::DirectionLight():
  lightDirection(vec3())
{
}

DirectionLight::DirectionLight(vec3 lightDirection):
  lightDirection(normalize(lightDirection))
{
}
void DirectionLight::bind_to_shader(const Shader & shader) const
{
  shader.set_vec3("LightDirection", lightDirection);
}
void DirectionLight::unbind_to_shader(const Shader & shader) const
{
  shader.set_vec3("LightDirection", vec3());
}