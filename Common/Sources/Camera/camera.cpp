#include "camera.h"
#include <vector>
#include "Application/application.h"

static vector<CameraPtr> cameras;


void Camera::set_orthographic(float width, float height, float zNear, float zFar) 
{ 
  const float aspectRatio = width / height;
  projection = ortho(-1.f, 1.f,-1.f * aspectRatio, 1.f * aspectRatio, zNear, zFar); 
}
void Camera::set_perspective(float fieldOfView, float aspectRatio, float zNear, float zFar)
{ 
  projection = perspective(fieldOfView, aspectRatio, zNear, zFar);
}

void Camera::set_perspective(float fieldOfView, float zNear, float zFar)
{
  const float aspectRatio = (float)Application::get_context().get_width() / Application::get_context().get_height();
  projection = perspective(fieldOfView, aspectRatio, zNear, zFar);
}
const mat4x4& Camera::get_projection() const
{
  return projection;
}
Transform& Camera::get_transform()
{
  return transform;
}
mat4x4 Camera::get_transform_matrix() const
{
  return transform.get_transform();
}
void Camera::set_priority(int priority)
{
  this->priority = priority;
}
int Camera::get_priority() const
{
  return priority;
}

void Camera::set_to_shader(const Shader& shader, bool sky_box) const
{
  vec3 cameraPosition = transform.get_position();
  mat4 view = inverse(transform.get_transform());

  mat4x4 viewProjection = projection * (sky_box ? mat4(mat3(view)): view);
  shader.set_mat4x4("ViewProjection", viewProjection);
  shader.set_vec3("CameraPosition", cameraPosition);
}

void add_camera(CameraPtr camera)
{
  cameras.emplace_back(camera);
}
CameraPtr main_camera()
{
  CameraPtr camera;
  if (cameras.size() > 0)
  {
    int max_priority = cameras[0]->get_priority();
    camera = cameras[0];
    for (auto camera_ptr : cameras)
    {
      if (camera_ptr->get_priority() > max_priority)
      {
        max_priority = camera_ptr->get_priority();
        camera = camera_ptr;
      }
    }
  }
  return camera;
}